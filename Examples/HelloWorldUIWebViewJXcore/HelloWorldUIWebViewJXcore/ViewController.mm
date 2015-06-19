//
//  ViewController.m
//  HelloWorldUIWebViewJXcore
//
//  Created by shailesh on 14/06/15.
//  Copyright (c) 2015 shailesh. All rights reserved.
//
#import "ViewController.h"



@interface ViewController ()


@end

@implementation ViewController
- (NSString*) getPathForDirectory:(NSSearchPathDirectory)directory {
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


- (void)viewDidLoad {
  [super viewDidLoad];
  [self copyFromBundleToDocuments:YES];
  self.embeddedJXcoreEngineIOS =  [[EmbeddedJXcoreEngineIOS alloc] init];
  NSString* documentsDirectory = [self getPathForDirectory:NSDocumentDirectory];
  NSString* mainFilePath = [documentsDirectory stringByAppendingString:@"/main.jx"];
  [[self embeddedJXcoreEngineIOS] setDelegate:self];
  [[self embeddedJXcoreEngineIOS] startEngine:mainFilePath homeFolder:documentsDirectory];

  
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}


-(void)webViewDidFinishLoad:(UIWebView *)webView{

}

-(void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error{
  NSLog(@"Failed with error %@",error.localizedDescription);
}

-(void)embeddedJXcoreEngineIOSDidStart{
  // Do any additional setup after loading the view, typically from a nib.
  NSURL *url = [NSURL URLWithString:@"http://localhost:3000"];
  NSURLRequest *request  = [NSURLRequest requestWithURL:url];
  //  _webView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
  _webView.backgroundColor = [UIColor greenColor];
  [[self webView] loadRequest:request];
  [[self webView] setDelegate:self];
}

-(void)embeddedJXcoreEngineIOSWillStop{
  
}

-(void)viewWillDisappear:(BOOL)animated{
  [_embeddedJXcoreEngineIOS stopEngine];
}

@end
