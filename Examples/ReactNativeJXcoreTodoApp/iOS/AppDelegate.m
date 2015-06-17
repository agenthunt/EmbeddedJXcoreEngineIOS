/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "AppDelegate.h"

#import "RCTRootView.h"

@implementation AppDelegate


- (NSString*) getPathForDirectory:(int)directory {
  NSArray *paths = NSSearchPathForDirectoriesInDomains(directory, NSUserDomainMask, YES);
  return [paths objectAtIndex:0];
}

- (void)copyFromBundleToDocuments:(BOOL)overWrite {
  NSString *appBundleDirectory = [[NSBundle mainBundle] resourcePath];
  NSString* documentsDirectory = [self getPathForDirectory:NSDocumentDirectory];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSError *error =  nil;
  NSArray* itemsToCopy = [fileManager contentsOfDirectoryAtPath:appBundleDirectory error:&error];
  if(error){
    NSLog(@"Failed to get contents of App Bundle");
    return;
  }
  for (NSString* item in itemsToCopy) {
    NSString *sourceDir = [appBundleDirectory stringByAppendingFormat:@"/%@",item];
    NSString *destDir = [documentsDirectory stringByAppendingFormat:@"/%@",item];
    BOOL isDirectory;
    BOOL fileExists = [fileManager fileExistsAtPath:destDir isDirectory:&isDirectory];
    if (fileExists && overWrite) {
      [fileManager removeItemAtPath:destDir error:&error];
      if(error && error.code != NSFileNoSuchFileError && error.code != NSFileWriteFileExistsError){
        NSDictionary *userInfo = [error userInfo];
        NSString *errorString = [[userInfo objectForKey:NSUnderlyingErrorKey] localizedDescription];
        NSLog(@"Delete File Failed: %@. %s, %i", errorString, __PRETTY_FUNCTION__, __LINE__);
      }
    }
    //copyItemAtPath doesnt succeed if file already exists
    BOOL copySuccess =  [fileManager copyItemAtPath:sourceDir toPath:destDir error:&error];
    if (copySuccess) {
      NSString* theFileName = [[destDir lastPathComponent] stringByDeletingPathExtension];
      NSLog(@"Successfully copied: %@", theFileName);
    }
  }
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  [self copyFromBundleToDocuments:NO];
  self.embeddedJXcoreEngineIOS =  [[EmbeddedJXcoreEngineIOS alloc] init];
  NSString* documentsDirectory = [self getPathForDirectory:NSDocumentDirectory];
  NSString* mainFilePath = [documentsDirectory stringByAppendingString:@"/main.jx"];
  [[self embeddedJXcoreEngineIOS] setDelegate:self];
  [[self embeddedJXcoreEngineIOS] startEngine:mainFilePath homeFolder:documentsDirectory];
  self.launchOptions = launchOptions;
  
  
  return YES;
}

- (void)applicationWillTerminate:(UIApplication *)application {
  // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
  [[self embeddedJXcoreEngineIOS] stopEngine];
}

- (void)embeddedJXcoreEngineIOSDidStart{
  NSURL *jsCodeLocation;
  
  /**
   * Loading JavaScript code - uncomment the one you want.
   *
   * OPTION 1
   * Load from development server. Start the server from the repository root:
   *
   * $ npm start
   *
   * To run on device, change `localhost` to the IP address of your computer
   * (you can get this by typing `ifconfig` into the terminal and selecting the
   * `inet` value under `en0:`) and make sure your computer and iOS device are
   * on the same Wi-Fi network.
   */
  
  jsCodeLocation = [NSURL URLWithString:@"http://localhost:8081/index.ios.bundle"];
  
  /**
   * OPTION 2
   * Load from pre-bundled file on disk. To re-generate the static bundle
   * from the root of your project directory, run
   *
   * $ react-native bundle --minify
   *
   * see http://facebook.github.io/react-native/docs/runningondevice.html
   */
  
  //   jsCodeLocation = [[NSBundle mainBundle] URLForResource:@"main" withExtension:@"jsbundle"];
  
  RCTRootView *rootView = [[RCTRootView alloc] initWithBundleURL:jsCodeLocation
                                                      moduleName:@"ReactNativeJXcoreTodoApp"
                                                   launchOptions:[self launchOptions]];
  
  self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
  UIViewController *rootViewController = [[UIViewController alloc] init];
  rootViewController.view = rootView;
  self.window.rootViewController = rootViewController;
  [self.window makeKeyAndVisible];
}

@end
