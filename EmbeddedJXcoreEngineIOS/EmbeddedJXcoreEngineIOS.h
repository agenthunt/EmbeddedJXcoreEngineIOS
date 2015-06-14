//
//  EmbeddedJXcoreEngineIOS.h
//  EmbeddedJXcoreEngineIOS
//
//  Created by shailesh on 13/06/15.
//  Copyright (c) 2015 shailesh. All rights reserved.
//

#import <Foundation/Foundation.h>
@interface EmbeddedJXcoreEngineIOS : NSObject


-(void) startEngine:(NSString*)mainFilePath homeFolder:(NSString*)homeFolder;
-(void)stopEngine;
@end
