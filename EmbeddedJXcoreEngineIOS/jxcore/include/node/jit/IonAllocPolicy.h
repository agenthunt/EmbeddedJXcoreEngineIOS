/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_IonAllocPolicy_h
#define jit_IonAllocPolicy_h

#include "mozilla/GuardObjects.h"
#include "mozilla/TypeTraits.h"

#include "jscntxt.h"

#include "ds/LifoAlloc.h"
#include "jit/InlineList.h"
#include "jit/Ion.h"

namespace js {
namespace jit {

class TempAllocator
{
    LifoAllocScope lifoScope_;

  public:
    explicit TempAllocator(LifoAlloc *lifoAlloc)
      : lifoScope_(lifoAlloc)
    { }

    void *allocateInfallible(size_t bytes)
    {
        return lifoScope_.alloc().allocInfallible(bytes);
    }

    void *allocate(size_t bytes)
    {
        void *p = lifoScope_.alloc().alloc(bytes);
        if (!ensureBallast())
            return nullptr;
        return p;
    }

    template <size_t ElemSize>
    void *allocateArray(size_t n)
    {
        if (n & mozilla::tl::MulOverflowMask<ElemSize>::value)
            return nullptr;
        void *p = lifoScope_.alloc().alloc(n * ElemSize);
        if (!ensureBallast())
            return nullptr;
        return p;
    }

    LifoAlloc *lifoAlloc()
    {
        return &lifoScope_.alloc();
    }

    bool ensureBallast() {
        // Most infallible Ion allocations are small, so we use a ballast of
        // ~16K for now.
        return lifoScope_.alloc().ensureUnusedApproximate(16 * 1024);
    }
};

class IonAllocPolicy
{
    TempAllocator &alloc_;

  public:
    MOZ_IMPLICIT IonAllocPolicy(TempAllocator &alloc)
      : alloc_(alloc)
    {}
    template <typename T>
    T *pod_malloc(size_t numElems) {
        if (numElems & mozilla::tl::MulOverflowMask<sizeof(T)>::value)
            return nullptr;
        return static_cast<T *>(alloc_.allocate(numElems * sizeof(T)));
    }
    template <typename T>
    T *pod_calloc(size_t numElems) {
        T *p = pod_malloc<T>(numElems);
        if (p)
            memset(p, 0, numElems * sizeof(T));
        return p;
    }
    template <typename T>
    T *pod_realloc(T *p, size_t oldSize, size_t newSize) {
        T *n = pod_malloc<T>(newSize);
        if (!n)
            return n;
        MOZ_ASSERT(!(oldSize & mozilla::tl::MulOverflowMask<sizeof(T)>::value));
        memcpy(n, p, Min(oldSize * sizeof(T), newSize * sizeof(T)));
        return n;
    }
    void free_(void *p) {
    }
    void reportAllocOverflow() const {
    }
};

class OldIonAllocPolicy
{
  public:
    OldIonAllocPolicy()
    {}
    template <typename T>
    T *pod_malloc(size_t numElems) {
        if (numElems & mozilla::tl::MulOverflowMask<sizeof(T)>::value)
            return nullptr;
        return static_cast<T *>(GetIonContext()->temp->allocate(numElems * sizeof(T)));
    }
    void free_(void *p) {
    }
    void reportAllocOverflow() const {
    }
};

class AutoIonContextAlloc
{
    TempAllocator tempAlloc_;
    IonContext *icx_;
    TempAllocator *prevAlloc_;

  public:
    explicit AutoIonContextAlloc(JSContext *cx)
      : tempAlloc_(&cx->tempLifoAlloc()),
        icx_(GetIonContext()),
        prevAlloc_(icx_->temp)
    {
        icx_->temp = &tempAlloc_;
    }

    ~AutoIonContextAlloc() {
        JS_ASSERT(icx_->temp == &tempAlloc_);
        icx_->temp = prevAlloc_;
    }
};

struct TempObject
{
    inline void *operator new(size_t nbytes, TempAllocator &alloc) {
        return alloc.allocateInfallible(nbytes);
    }
    template <class T>
    inline void *operator new(size_t nbytes, T *pos) {
        static_assert(mozilla::IsConvertible<T*, TempObject*>::value,
                      "Placement new argument type must inherit from TempObject");
        return pos;
    }
};

template <typename T>
class TempObjectPool
{
    TempAllocator *alloc_;
    InlineForwardList<T> freed_;

  public:
    TempObjectPool()
      : alloc_(nullptr)
    {}
    void setAllocator(TempAllocator &alloc) {
        JS_ASSERT(freed_.empty());
        alloc_ = &alloc;
    }
    T *allocate() {
        JS_ASSERT(alloc_);
        if (freed_.empty())
            return new(*alloc_) T();
        return freed_.popFront();
    }
    void free(T *obj) {
        freed_.pushFront(obj);
    }
    void clear() {
        freed_.clear();
    }
};

} // namespace jit
} // namespace js

#endif /* jit_IonAllocPolicy_h */
