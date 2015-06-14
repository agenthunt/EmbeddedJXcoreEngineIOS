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
  JX_DefineMainFile((char*)byteData);
  
  
  JX_StartEngine();
  
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
  JX_StopEngine();
}

@end
