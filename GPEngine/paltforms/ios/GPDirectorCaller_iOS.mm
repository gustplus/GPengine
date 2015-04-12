//
//  GPDirectorCaller.m
//  OpenGLES3
//
//  Created by zh on 14-11-3.
//  Copyright (c) 2014年 com.gp. All rights reserved.
//

#import "GPDirectorCaller_iOS.h"
#include "glfunc.h"
#include "GPDirector.h"

static id s_instance;

@implementation GPDirectorCaller

+(id) getInstance
{
    if (s_instance == nil)
    {
        s_instance = [[GPDirectorCaller alloc] init];
    }
    return s_instance;
}

-(id) init
{
    self = [super init];
    if (self)
    {
        _timeINterval = 1.0/60.0;
    }
    return self;
}

-(void) setTimeInterval:(float)timeInterval
{
    [self startMainLoop];
}

-(void) startMainLoop
{
    [self stopMainLoop];
    
    _displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];
    [_displayLink setFrameInterval:_timeINterval];
    [_displayLink addToRunLoop: [NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) stopMainLoop
{
    if (_displayLink)
    {
        [_displayLink invalidate];
        _displayLink = nil;
    }
}

-(void) doCaller:(id) sender
{
    GPEngine3D::Director::getInstance()->mainLoop();
}

@end
