//
//  GPDirectorCaller.h
//  OpenGLES3
//
//  Created by zh on 14-11-3.
//  Copyright (c) 2014年 com.gp. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GPDirectorCaller : NSObject {
    id _displayLink;
    float _timeINterval;
}

-(void) setTimeInterval:(float)timeInterval;

-(void) startMainLoop;

-(void) stopMainLoop;

-(void) doCaller:(id)sender;

+(id) getInstance;
@end
