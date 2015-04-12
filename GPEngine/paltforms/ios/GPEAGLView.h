//
//  GPGLView.h
//  OpenGLES3
//
//  Created by zh on 14-11-3.
//  Copyright (c) 2014年 com.gp. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "glfunc.h"
#import "GPESRender.h"

@interface GPEAGLView : UIView
{
    GPESRender *_render;
    
    CAEAGLLayer *_renderLayer;
    GLuint _drawBuffer;
    GLuint _colorBuffer;
    
    BOOL _multiSampling;
}

-(id) initWithFrame:(CGRect)frame PixelFormat:(PixelFormat) pixelFormat DepthFormat:(DepthFormat) depthFormat multiSampling:(BOOL) multiSampling WithNumOfSamples:(unsigned int) num;

-(void) swapBuffer;

-(void) layoutSubviews;

-(int) getWidth;

-(int) getHeight;
@end
