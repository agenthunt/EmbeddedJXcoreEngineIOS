//
//  EmbeddedJXcoreEngineIOS.h
//  EmbeddedJXcoreEngineIOS
//
//  Created by shailesh on 13/06/15.
//  Copyright (c) 2015 shailesh. All rights reserved.
//

#import <Foundation/Foundation.h>
@protocol EmbeddedJXcoreEngineIOSDelegate <NSObject>
@optional
- (void)embeddedJXcoreEngineIOSDidStart;
- (void)embeddedJXcoreEngineIOSWillStop;
@end

@interface EmbeddedJXcoreEngineIOS : NSObject

@property (nonatomic, weak) id <EmbeddedJXcoreEngineIOSDelegate> delegate;

-(void) startEngine:(NSString*)mainFilePath homeFolder:(NSString*)homeFolder;
-(void)stopEngine;
@end
