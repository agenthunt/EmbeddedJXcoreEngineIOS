/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_x86_Assembler_x86_h
#define jit_x86_Assembler_x86_h

#include "mozilla/ArrayUtils.h"

#include "jit/CompactBuffer.h"
#include "jit/IonCode.h"
#include "jit/JitCompartment.h"
#include "jit/shared/Assembler-shared.h"
#include "jit/shared/BaseAssembler-x86-shared.h"

namespace js {
namespace jit {

static MOZ_CONSTEXPR_VAR Register eax = { X86Registers::eax };
static MOZ_CONSTEXPR_VAR Register ecx = { X86Registers::ecx };
static MOZ_CONSTEXPR_VAR Register edx = { X86Registers::edx };
static MOZ_CONSTEXPR_VAR Register ebx = { X86Registers::ebx };
static MOZ_CONSTEXPR_VAR Register esp = { X86Registers::esp };
static MOZ_CONSTEXPR_VAR Register ebp = { X86Registers::ebp };
static MOZ_CONSTEXPR_VAR Register esi = { X86Registers::esi };
static MOZ_CONSTEXPR_VAR Register edi = { X86Registers::edi };

static MOZ_CONSTEXPR_VAR FloatRegister xmm0 = { X86Registers::xmm0 };
static MOZ_CONSTEXPR_VAR FloatRegister xmm1 = { X86Registers::xmm1 };
static MOZ_CONSTEXPR_VAR FloatRegister xmm2 = { X86Registers::xmm2 };
static MOZ_CONSTEXPR_VAR FloatRegister xmm3 = { X86Registers::xmm3 };
static MOZ_CONSTEXPR_VAR FloatRegister xmm4 = { X86Registers::xmm4 };
static MOZ_CONSTEXPR_VAR FloatRegister xmm5 = { X86Registers::xmm5 };
static MOZ_CONSTEXPR_VAR FloatRegister xmm6 = { X86Registers::xmm6 };
static MOZ_CONSTEXPR_VAR FloatRegister xmm7 = { X86Registers::xmm7 };

static MOZ_CONSTEXPR_VAR Register InvalidReg = { X86Registers::invalid_reg };
static MOZ_CONSTEXPR_VAR FloatRegister InvalidFloatReg = { X86Registers::invalid_xmm };

static MOZ_CONSTEXPR_VAR Register JSReturnReg_Type = ecx;
static MOZ_CONSTEXPR_VAR Register JSReturnReg_Data = edx;
static MOZ_CONSTEXPR_VAR Register StackPointer = esp;
static MOZ_CONSTEXPR_VAR Register FramePointer = ebp;
static MOZ_CONSTEXPR_VAR Register ReturnReg = eax;
static MOZ_CONSTEXPR_VAR FloatRegister ReturnFloat32Reg = xmm0;
static MOZ_CONSTEXPR_VAR FloatRegister ScratchFloat32Reg = xmm7;
static MOZ_CONSTEXPR_VAR FloatRegister ReturnDoubleReg = xmm0;
static MOZ_CONSTEXPR_VAR FloatRegister ScratchDoubleReg = xmm7;
static MOZ_CONSTEXPR_VAR FloatRegister ReturnSimdReg = xmm0;
static MOZ_CONSTEXPR_VAR FloatRegister ScratchSimdReg = xmm7;

// Avoid ebp, which is the FramePointer, which is unavailable in some modes.
static MOZ_CONSTEXPR_VAR Register ArgumentsRectifierReg = esi;
static MOZ_CONSTEXPR_VAR Register CallTempReg0 = edi;
static MOZ_CONSTEXPR_VAR Register CallTempReg1 = eax;
static MOZ_CONSTEXPR_VAR Register CallTempReg2 = ebx;
static MOZ_CONSTEXPR_VAR Register CallTempReg3 = ecx;
static MOZ_CONSTEXPR_VAR Register CallTempReg4 = esi;
static MOZ_CONSTEXPR_VAR Register CallTempReg5 = edx;

// The convention used by the ForkJoinGetSlice stub. None of these can be eax
// or edx, which the stub also needs for cmpxchg and div, respectively.
static MOZ_CONSTEXPR_VAR Register ForkJoinGetSliceReg_cx = edi;
static MOZ_CONSTEXPR_VAR Register ForkJoinGetSliceReg_temp0 = ebx;
static MOZ_CONSTEXPR_VAR Register ForkJoinGetSliceReg_temp1 = ecx;
static MOZ_CONSTEXPR_VAR Register ForkJoinGetSliceReg_output = esi;

// We have no arg regs, so our NonArgRegs are just our CallTempReg*
static MOZ_CONSTEXPR_VAR Register CallTempNonArgRegs[] = { edi, eax, ebx, ecx, esi, edx };
static const uint32_t NumCallTempNonArgRegs =
    mozilla::ArrayLength(CallTempNonArgRegs);

class ABIArgGenerator
{
    uint32_t stackOffset_;
    ABIArg current_;

  public:
    ABIArgGenerator();
    ABIArg next(MIRType argType);
    ABIArg &current() { return current_; }
    uint32_t stackBytesConsumedSoFar() const { return stackOffset_; }

    // Note: these registers are all guaranteed to be different
    static const Register NonArgReturnReg0;
    static const Register NonArgReturnReg1;
    static const Register NonVolatileReg;
    static const Register NonArg_VolatileReg;
    static const Register NonReturn_VolatileReg0;
};

static MOZ_CONSTEXPR_VAR Register OsrFrameReg = edx;
static MOZ_CONSTEXPR_VAR Register PreBarrierReg = edx;

// Registers used in the GenerateFFIIonExit Enable Activation block.
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegCallee = ecx;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegE0 = edi;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegE1 = eax;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegE2 = ebx;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegE3 = edx;

// Registers used in the GenerateFFIIonExit Disable Activation block.
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegReturnData = edx;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegReturnType = ecx;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegD0 = edi;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegD1 = eax;
static MOZ_CONSTEXPR_VAR Register AsmJSIonExitRegD2 = esi;

// GCC stack is aligned on 16 bytes. Ion does not maintain this for internal
// calls. asm.js code does.
#if defined(__GNUC__)
static const uint32_t ABIStackAlignment = 16;
#else
static const uint32_t ABIStackAlignment = 4;
#endif
static const uint32_t CodeAlignment = 8;

// This boolean indicates whether we support SIMD instructions flavoured for
// this architecture or not. Rather than a method in the LIRGenerator, it is
// here such that it is accessible from the entire codebase. Once full support
// for SIMD is reached on all tier-1 platforms, this constant can be deleted.
static const bool SupportsSimd = true;
static const uint32_t SimdStackAlignment = 16;

static const uint32_t AsmJSStackAlignment = SimdStackAlignment;

struct ImmTag : public Imm32
{
    ImmTag(JSValueTag mask)
      : Imm32(int32_t(mask))
    { }
};

struct ImmType : public ImmTag
{
    ImmType(JSValueType type)
      : ImmTag(JSVAL_TYPE_TO_TAG(type))
    { }
};

static const Scale ScalePointer = TimesFour;

} // namespace jit
} // namespace js

#include "jit/shared/Assembler-x86-shared.h"

namespace js {
namespace jit {

static inline void
PatchJump(CodeLocationJump jump, CodeLocationLabel label)
{
#ifdef DEBUG
    // Assert that we're overwriting a jump instruction, either:
    //   0F 80+cc <imm32>, or
    //   E9 <imm32>
    unsigned char *x = (unsigned char *)jump.raw() - 5;
    JS_ASSERT(((*x >= 0x80 && *x <= 0x8F) && *(x - 1) == 0x0F) ||
              (*x == 0xE9));
#endif
    X86Assembler::setRel32(jump.raw(), label.raw());
}
static inline void
PatchBackedge(CodeLocationJump &jump_, CodeLocationLabel label, JitRuntime::BackedgeTarget target)
{
    PatchJump(jump_, label);
}

// Return operand from a JS -> JS call.
static const ValueOperand JSReturnOperand = ValueOperand(JSReturnReg_Type, JSReturnReg_Data);

class Assembler : public AssemblerX86Shared
{
    void writeRelocation(JmpSrc src) {
        jumpRelocations_.writeUnsigned(src.offset());
    }
    void addPendingJump(JmpSrc src, ImmPtr target, Relocation::Kind kind) {
        enoughMemory_ &= jumps_.append(RelativePatch(src.offset(), target.value, kind));
        if (kind == Relocation::JITCODE)
            writeRelocation(src);
    }

  public:
    using AssemblerX86Shared::movl;
    using AssemblerX86Shared::j;
    using AssemblerX86Shared::jmp;
    using AssemblerX86Shared::movsd;
    using AssemblerX86Shared::movss;
    using AssemblerX86Shared::retarget;
    using AssemblerX86Shared::cmpl;
    using AssemblerX86Shared::call;
    using AssemblerX86Shared::push;
    using AssemblerX86Shared::pop;

    static void TraceJumpRelocations(JSTracer *trc, JitCode *code, CompactBufferReader &reader);

    // Copy the assembly code to the given buffer, and perform any pending
    // relocations relying on the target address.
    void executableCopy(uint8_t *buffer);

    // Actual assembly emitting functions.

    void push(ImmGCPtr ptr) {
        push(Imm32(uintptr_t(ptr.value)));
        writeDataRelocation(ptr);
    }
    void push(ImmMaybeNurseryPtr ptr) {
        push(noteMaybeNurseryPtr(ptr));
    }
    void push(const ImmWord imm) {
        push(Imm32(imm.value));
    }
    void push(const ImmPtr imm) {
        push(ImmWord(uintptr_t(imm.value)));
    }
    void push(FloatRegister src) {
        subl(Imm32(sizeof(double)), StackPointer);
        movsd(src, Address(StackPointer, 0));
    }

    CodeOffsetLabel pushWithPatch(ImmWord word) {
        push(Imm32(word.value));
        return CodeOffsetLabel(masm.currentOffset());
    }

    void pop(FloatRegister src) {
        movsd(Address(StackPointer, 0), src);
        addl(Imm32(sizeof(double)), StackPointer);
    }

    CodeOffsetLabel movWithPatch(ImmWord word, Register dest) {
        movl(Imm32(word.value), dest);
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movWithPatch(ImmPtr imm, Register dest) {
        return movWithPatch(ImmWord(uintptr_t(imm.value)), dest);
    }

    void movl(ImmGCPtr ptr, Register dest) {
        masm.movl_i32r(uintptr_t(ptr.value), dest.code());
        writeDataRelocation(ptr);
    }
    void movl(ImmGCPtr ptr, const Operand &dest) {
        switch (dest.kind()) {
          case Operand::REG:
            masm.movl_i32r(uintptr_t(ptr.value), dest.reg());
            writeDataRelocation(ptr);
            break;
          case Operand::MEM_REG_DISP:
            masm.movl_i32m(uintptr_t(ptr.value), dest.disp(), dest.base());
            writeDataRelocation(ptr);
            break;
          case Operand::MEM_SCALE:
            masm.movl_i32m(uintptr_t(ptr.value), dest.disp(), dest.base(), dest.index(), dest.scale());
            writeDataRelocation(ptr);
            break;
          default:
            MOZ_CRASH("unexpected operand kind");
        }
    }
    void movl(ImmWord imm, Register dest) {
        masm.movl_i32r(imm.value, dest.code());
    }
    void movl(ImmPtr imm, Register dest) {
        movl(ImmWord(uintptr_t(imm.value)), dest);
    }
    void mov(ImmWord imm, Register dest) {
        // Use xor for setting registers to zero, as it is specially optimized
        // for this purpose on modern hardware. Note that it does clobber FLAGS
        // though.
        if (imm.value == 0)
            xorl(dest, dest);
        else
            movl(imm, dest);
    }
    void mov(ImmPtr imm, Register dest) {
        mov(ImmWord(uintptr_t(imm.value)), dest);
    }
    void mov(AsmJSImmPtr imm, Register dest) {
        masm.movl_i32r(-1, dest.code());
        append(AsmJSAbsoluteLink(CodeOffsetLabel(masm.currentOffset()), imm.kind()));
    }
    void mov(const Operand &src, Register dest) {
        movl(src, dest);
    }
    void mov(Register src, const Operand &dest) {
        movl(src, dest);
    }
    void mov(Imm32 imm, const Operand &dest) {
        movl(imm, dest);
    }
    void mov(AbsoluteLabel *label, Register dest) {
        JS_ASSERT(!label->bound());
        // Thread the patch list through the unpatched address word in the
        // instruction stream.
        masm.movl_i32r(label->prev(), dest.code());
        label->setPrev(masm.size());
    }
    void mov(Register src, Register dest) {
        movl(src, dest);
    }
    void xchg(Register src, Register dest) {
        xchgl(src, dest);
    }
    void lea(const Operand &src, Register dest) {
        return leal(src, dest);
    }

    void fld32(const Operand &dest) {
        switch (dest.kind()) {
          case Operand::MEM_REG_DISP:
            masm.fld32_m(dest.disp(), dest.base());
            break;
          default:
            MOZ_CRASH("unexpected operand kind");
        }
    }

    void fstp32(const Operand &src) {
        switch (src.kind()) {
          case Operand::MEM_REG_DISP:
            masm.fstp32_m(src.disp(), src.base());
            break;
          default:
            MOZ_CRASH("unexpected operand kind");
        }
    }

    void cmpl(const Register src, ImmWord ptr) {
        masm.cmpl_ir(ptr.value, src.code());
    }
    void cmpl(const Register src, ImmPtr imm) {
        cmpl(src, ImmWord(uintptr_t(imm.value)));
    }
    void cmpl(const Register src, ImmGCPtr ptr) {
        masm.cmpl_ir(uintptr_t(ptr.value), src.code());
        writeDataRelocation(ptr);
    }
    void cmpl(Register lhs, Register rhs) {
        masm.cmpl_rr(rhs.code(), lhs.code());
    }
    void cmpl(const Operand &op, ImmGCPtr imm) {
        switch (op.kind()) {
          case Operand::REG:
            masm.cmpl_ir_force32(uintptr_t(imm.value), op.reg());
            writeDataRelocation(imm);
            break;
          case Operand::MEM_REG_DISP:
            masm.cmpl_im_force32(uintptr_t(imm.value), op.disp(), op.base());
            writeDataRelocation(imm);
            break;
          case Operand::MEM_ADDRESS32:
            masm.cmpl_im(uintptr_t(imm.value), op.address());
            writeDataRelocation(imm);
            break;
          default:
            MOZ_CRASH("unexpected operand kind");
        }
    }
    void cmpl(const Operand &op, ImmMaybeNurseryPtr imm) {
        cmpl(op, noteMaybeNurseryPtr(imm));
    }
    void cmpl(AsmJSAbsoluteAddress lhs, Register rhs) {
        masm.cmpl_rm_force32(rhs.code(), (void*)-1);
        append(AsmJSAbsoluteLink(CodeOffsetLabel(masm.currentOffset()), lhs.kind()));
    }

    void jmp(ImmPtr target, Relocation::Kind reloc = Relocation::HARDCODED) {
        JmpSrc src = masm.jmp();
        addPendingJump(src, target, reloc);
    }
    void j(Condition cond, ImmPtr target,
           Relocation::Kind reloc = Relocation::HARDCODED) {
        JmpSrc src = masm.jCC(static_cast<X86Assembler::Condition>(cond));
        addPendingJump(src, target, reloc);
    }

    void jmp(JitCode *target) {
        jmp(ImmPtr(target->raw()), Relocation::JITCODE);
    }
    void j(Condition cond, JitCode *target) {
        j(cond, ImmPtr(target->raw()), Relocation::JITCODE);
    }
    void call(JitCode *target) {
        JmpSrc src = masm.call();
        addPendingJump(src, ImmPtr(target->raw()), Relocation::JITCODE);
    }
    void call(ImmWord target) {
        call(ImmPtr((void*)target.value));
    }
    void call(ImmPtr target) {
        JmpSrc src = masm.call();
        addPendingJump(src, target, Relocation::HARDCODED);
    }

    // Emit a CALL or CMP (nop) instruction. ToggleCall can be used to patch
    // this instruction.
    CodeOffsetLabel toggledCall(JitCode *target, bool enabled) {
        CodeOffsetLabel offset(size());
        JmpSrc src = enabled ? masm.call() : masm.cmp_eax();
        addPendingJump(src, ImmPtr(target->raw()), Relocation::JITCODE);
        JS_ASSERT(size() - offset.offset() == ToggledCallSize(nullptr));
        return offset;
    }

    static size_t ToggledCallSize(uint8_t *code) {
        // Size of a call instruction.
        return 5;
    }

    // Re-routes pending jumps to an external target, flushing the label in the
    // process.
    void retarget(Label *label, ImmPtr target, Relocation::Kind reloc) {
        if (label->used()) {
            bool more;
            X86Assembler::JmpSrc jmp(label->offset());
            do {
                X86Assembler::JmpSrc next;
                more = masm.nextJump(jmp, &next);
                addPendingJump(jmp, target, reloc);
                jmp = next;
            } while (more);
        }
        label->reset();
    }

    // Move a 32-bit immediate into a register where the immediate can be
    // patched.
    CodeOffsetLabel movlWithPatch(Imm32 imm, Register dest) {
        masm.movl_i32r(imm.value, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }

    // Load from *(base + disp32) where disp32 can be patched.
    CodeOffsetLabel movsblWithPatch(Address src, Register dest) {
        masm.movsbl_mr_disp32(src.offset, src.base.code(), dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movzblWithPatch(Address src, Register dest) {
        masm.movzbl_mr_disp32(src.offset, src.base.code(), dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movswlWithPatch(Address src, Register dest) {
        masm.movswl_mr_disp32(src.offset, src.base.code(), dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movzwlWithPatch(Address src, Register dest) {
        masm.movzwl_mr_disp32(src.offset, src.base.code(), dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movlWithPatch(Address src, Register dest) {
        masm.movl_mr_disp32(src.offset, src.base.code(), dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movssWithPatch(Address src, FloatRegister dest) {
        JS_ASSERT(HasSSE2());
        masm.movss_mr_disp32(src.offset, src.base.code(), dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movsdWithPatch(Address src, FloatRegister dest) {
        JS_ASSERT(HasSSE2());
        masm.movsd_mr_disp32(src.offset, src.base.code(), dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }

    // Store to *(base + disp32) where disp32 can be patched.
    CodeOffsetLabel movbWithPatch(Register src, Address dest) {
        masm.movb_rm_disp32(src.code(), dest.offset, dest.base.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movwWithPatch(Register src, Address dest) {
        masm.movw_rm_disp32(src.code(), dest.offset, dest.base.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movlWithPatch(Register src, Address dest) {
        masm.movl_rm_disp32(src.code(), dest.offset, dest.base.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movssWithPatch(FloatRegister src, Address dest) {
        JS_ASSERT(HasSSE2());
        masm.movss_rm_disp32(src.code(), dest.offset, dest.base.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movsdWithPatch(FloatRegister src, Address dest) {
        JS_ASSERT(HasSSE2());
        masm.movsd_rm_disp32(src.code(), dest.offset, dest.base.code());
        return CodeOffsetLabel(masm.currentOffset());
    }

    // Load from *(addr + index*scale) where addr can be patched.
    CodeOffsetLabel movlWithPatch(PatchedAbsoluteAddress addr, Register index, Scale scale,
                                  Register dest)
    {
        masm.movl_mr(addr.addr, index.code(), scale, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }

    // Load from *src where src can be patched.
    CodeOffsetLabel movsblWithPatch(PatchedAbsoluteAddress src, Register dest) {
        masm.movsbl_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movzblWithPatch(PatchedAbsoluteAddress src, Register dest) {
        masm.movzbl_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movswlWithPatch(PatchedAbsoluteAddress src, Register dest) {
        masm.movswl_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movzwlWithPatch(PatchedAbsoluteAddress src, Register dest) {
        masm.movzwl_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movlWithPatch(PatchedAbsoluteAddress src, Register dest) {
        masm.movl_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movssWithPatch(PatchedAbsoluteAddress src, FloatRegister dest) {
        JS_ASSERT(HasSSE2());
        masm.movss_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movsdWithPatch(PatchedAbsoluteAddress src, FloatRegister dest) {
        JS_ASSERT(HasSSE2());
        masm.movsd_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movdqaWithPatch(PatchedAbsoluteAddress src, FloatRegister dest) {
        JS_ASSERT(HasSSE2());
        masm.movdqa_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movapsWithPatch(PatchedAbsoluteAddress src, FloatRegister dest) {
        JS_ASSERT(HasSSE2());
        masm.movaps_mr(src.addr, dest.code());
        return CodeOffsetLabel(masm.currentOffset());
    }

    // Store to *dest where dest can be patched.
    CodeOffsetLabel movbWithPatch(Register src, PatchedAbsoluteAddress dest) {
        masm.movb_rm(src.code(), dest.addr);
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movwWithPatch(Register src, PatchedAbsoluteAddress dest) {
        masm.movw_rm(src.code(), dest.addr);
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movlWithPatch(Register src, PatchedAbsoluteAddress dest) {
        masm.movl_rm(src.code(), dest.addr);
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movssWithPatch(FloatRegister src, PatchedAbsoluteAddress dest) {
        JS_ASSERT(HasSSE2());
        masm.movss_rm(src.code(), dest.addr);
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movsdWithPatch(FloatRegister src, PatchedAbsoluteAddress dest) {
        JS_ASSERT(HasSSE2());
        masm.movsd_rm(src.code(), dest.addr);
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movdqaWithPatch(FloatRegister src, PatchedAbsoluteAddress dest) {
        JS_ASSERT(HasSSE2());
        masm.movdqa_rm(src.code(), dest.addr);
        return CodeOffsetLabel(masm.currentOffset());
    }
    CodeOffsetLabel movapsWithPatch(FloatRegister src, PatchedAbsoluteAddress dest) {
        JS_ASSERT(HasSSE2());
        masm.movaps_rm(src.code(), dest.addr);
        return CodeOffsetLabel(masm.currentOffset());
    }

    void loadAsmJSActivation(Register dest) {
        CodeOffsetLabel label = movlWithPatch(PatchedAbsoluteAddress(), dest);
        append(AsmJSGlobalAccess(label, AsmJSActivationGlobalDataOffset));
    }
};

// Get a register in which we plan to put a quantity that will be used as an
// integer argument.  This differs from GetIntArgReg in that if we have no more
// actual argument registers to use we will fall back on using whatever
// CallTempReg* don't overlap the argument registers, and only fail once those
// run out too.
static inline bool
GetTempRegForIntArg(uint32_t usedIntArgs, uint32_t usedFloatArgs, Register *out)
{
    if (usedIntArgs >= NumCallTempNonArgRegs)
        return false;
    *out = CallTempNonArgRegs[usedIntArgs];
    return true;
}

} // namespace jit
} // namespace js

#endif /* jit_x86_Assembler_x86_h */
