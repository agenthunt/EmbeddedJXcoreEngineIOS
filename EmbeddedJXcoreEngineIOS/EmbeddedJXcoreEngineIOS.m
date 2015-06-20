//
//  EmbeddedJXcoreEngineIOS.m
//  EmbeddedJXcoreEngineIOS
//
//  Created by shailesh on 13/06/15.
//  Copyright (c) 2015 shailesh. All rights reserved.
//

#import "EmbeddedJXcoreEngineIOS.h"

#include <stdlib.h>
#include <string.h>

// ugly but takes care of XCODE 6 i386 compile bug
size_t fwrite$UNIX2003(const void *a, size_t b, size_t c, FILE *d) {
  return fwrite(a, b, c, d);
}
char *strerror$UNIX2003(int errnum) { return strerror(errnum); }
time_t mktime$UNIX2003(struct tm *a) { return mktime(a); }
double strtod$UNIX2003(const char *a, char **b) { return strtod(a, b); }
void fputs$UNIX2003(const char *restrict c, FILE *restrict f) { fputs(c, f); }

#if defined(_MSC_VER)
// Sleep time for Windows is 1 ms while it's 1 ns for POSIX
// Beware using this for your app. This is just to give a
// basic idea on usage
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep(x)
#endif

#include "jx.h"

#define flush_console(...)        \
do {                            \
fprintf(stdout, __VA_ARGS__); \
fflush(stdout);               \
} while (0)

void callback(JXResult *results, int argc) {
  // do nothing
}

@implementation EmbeddedJXcoreEngineIOS
@synthesize delegate;
-(void)runEngineInSeparateThread:(NSDictionary*) args{
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSData *mainFileData =  [fileManager contentsAtPath:[args objectForKey:@"mainFilePath"]];
  NSUInteger len = [mainFileData length];
  Byte *byteData = (Byte*)malloc(len);
  memcpy(byteData, [mainFileData bytes], len);
  
  NSString   *homeFolder = [args objectForKey:@"homeFolder"];
  const char* path = [homeFolder UTF8String];
  NSLog(@"%s", path);
  
  // Call JX_Initialize only once per app
  
  JX_Initialize(path, callback);
  // Creates a new engine for the current thread
  // It's our first engine instance hence it will be the
  // parent engine for all the other engine instances.
  // If you need to destroy this engine instance, you should
  // destroy everything else first. For the sake of this sample
  // we have our first instance sitting on the main thread
  // and it will be destroyed when the app exists.
  JX_InitializeNewEngine();
  
  
  // define the entry file contents
  //JX_DefineMainFile((char*)byteData);
  JX_DefineMainFile("require('main.jx')");
  
  
  JX_StartEngine();
  dispatch_async(dispatch_get_main_queue(), ^{
    if (self.delegate && [self.delegate respondsToSelector:@selector(embeddedJXcoreEngineIOSDidStart)]) {
      [self.delegate embeddedJXcoreEngineIOSDidStart];
    }
  });
  
  // loop for possible IO
  // or JX_Loop() without usleep / while
  while (JX_LoopOnce() != 0) Sleep(1);
  
  
  // loop for possible IO
  // or JX_Loop() without usleep / while
  while (JX_LoopOnce() != 0) Sleep(1);
}

-(void) startEngine:(NSString*)mainFilePath homeFolder:(NSString*)homeFolder{
  NSDictionary * args = [NSDictionary dictionaryWithObjectsAndKeys:
                         mainFilePath, @"mainFilePath",
                         homeFolder, @"homeFolder",
                         nil];
  [self performSelectorInBackground:@selector(runEngineInSeparateThread:) withObject:args];
}


-(void) stopEngine{
  if (self.delegate && [self.delegate respondsToSelector:@selector(embeddedJXcoreEngineIOSWillStop)]) {
    [self.delegate embeddedJXcoreEngineIOSWillStop];
  }
  JX_StopEngine();
}

@end
