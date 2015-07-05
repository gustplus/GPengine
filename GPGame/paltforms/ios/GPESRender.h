//
//  GPRender.h
//  OpenGLES3
//
//  Created by zh on 14-11-3.
//  Copyright (c) 2014年 com.gp. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OpenGLES/ES3/gl.h"
#import "glfunc.h"

@interface GPESRender : NSObject
{
    EAGLContext *_context;
    
    GLuint _backFrameBuffer;
    GLuint _colorRenderBuffer;
    GLuint _depthRenderBuffer;
    
    GLuint _msaaFrameBuffer;
    GLuint _msaaColorBuffer;
    
    BOOL _isMultiSampling;
    unsigned int _numSmaples;
    
    GLenum _pixelFormat;
    GLenum _depthFormat;
    
    int _backWidth;
    int _backHeight;
}

@property (nonatomic,readonly) EAGLContext *context;

@property (nonatomic,readonly) int width;
@property (nonatomic,readonly) int height;
@property (nonatomic,readonly) GLuint msaaFrameBuffer;

-(id)initWithPixelFormat:(PixelFormat) pixelFormat DepthFormat:(DepthFormat) depthFormat multiSampling:(BOOL) multiSampling WithNumOfSamples:(unsigned int) num;

-(BOOL) resizeFrameBufferWithLayer:(CAEAGLLayer *) layer;

@end
