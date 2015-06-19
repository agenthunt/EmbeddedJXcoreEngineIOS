//
//  ViewController.h
//  HelloWorldUIWebViewJXcore
//
//  Created by shailesh on 14/06/15.
//  Copyright (c) 2015 shailesh. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EmbeddedJXcoreEngineIOS.h"
@interface ViewController : UIViewController <UIWebViewDelegate,EmbeddedJXcoreEngineIOSDelegate>

@property (weak, nonatomic) IBOutlet UIWebView *webView;
@property (strong, nonatomic) EmbeddedJXcoreEngineIOS *embeddedJXcoreEngineIOS;


@end

