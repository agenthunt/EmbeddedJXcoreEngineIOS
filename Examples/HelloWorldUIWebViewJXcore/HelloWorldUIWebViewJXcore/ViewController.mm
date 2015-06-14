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

- (void)viewDidLoad {
  [super viewDidLoad];
  // Do any additional setup after loading the view, typically from a nib.
  NSURL *url = [NSURL URLWithString:@"http://localhost:3000"];
  NSURLRequest *request  = [NSURLRequest requestWithURL:url];
//  _webView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
      _webView.backgroundColor = [UIColor greenColor];
  [[self webView] loadRequest:request];
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

@end
