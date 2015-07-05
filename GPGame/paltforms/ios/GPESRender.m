//
//  GPRender.m
//  OpenGLES3
//
//  Created by zh on 14-11-3.
//  Copyright (c) 2014年 com.gp. All rights reserved.
//

#import "GPESRender.h"

@implementation GPESRender

@synthesize context = _context;

@synthesize width = _backWidth;
@synthesize height = _backHeight;

@synthesize msaaFrameBuffer = _msaaFrameBuffer;

-(id)initWithPixelFormat:(PixelFormat) pixelFormat DepthFormat:(DepthFormat) depthFormat multiSampling:(BOOL) multiSampling WithNumOfSamples:(unsigned int) num
{
    self = [super init];
    if (self)
    {
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if(_context){
            if (![EAGLContext setCurrentContext:_context]) {
                NSLog(@"Failed to set current OpenGL context");
                exit(1);
            }
        }else{
            NSLog(@"Failed to create OpenGL context");
            exit(1);
        }
        
        _pixelFormat = pixelFormat;
        _depthFormat = depthFormat;
        _numSmaples = num;
        _isMultiSampling = multiSampling;
        
        [self setupFrameBuffer];
    }
    return self;
}

-(void) setupFrameBuffer
{
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    
    glGenFramebuffers(1, &_backFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _backFrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);

    if (_isMultiSampling)
    {
        GLint maxSampleNum;
        glGetIntegerv(GL_MAX_SAMPLES, &maxSampleNum);
        _numSmaples = MIN(maxSampleNum, _numSmaples);
        
        glGenFramebuffers(1, &_msaaFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _msaaFrameBuffer);
        printf("%d", _msaaFrameBuffer);
    }
}

-(BOOL) resizeFrameBufferWithLayer:(CAEAGLLayer *) layer
{
    GLenum error;
    if (_colorRenderBuffer == 0 || _backFrameBuffer == 0)
    {
        [self setupFrameBuffer];
    }
    if (_isMultiSampling && !_msaaFrameBuffer)
    {
        [self setupFrameBuffer];
    }
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    //    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, _width, _height);  *** i don't know why this function call doesn't work but the bottom one does
    if (![_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer]){
        NSLog(@"failed to call centext!");
    }
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backHeight);
    NSLog(@"surface size = %d x %d", _backWidth, _backHeight);
    
    if (_depthFormat != 0)
    {
        if (!_depthRenderBuffer)
        {
            glGenRenderbuffers(1, &_depthRenderBuffer);
            NSAssert(_depthRenderBuffer, @"can't create depth buffer");
        }
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
        if (_isMultiSampling)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, _numSmaples, _depthFormat, _backWidth, _backHeight);
        }else{
            glRenderbufferStorage(GL_RENDERBUFFER, _depthFormat, _backWidth, _backHeight);
        }
//        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
        if (_depthFormat == DEPTH24_STENCIL8 || _depthFormat == DEPTH32F_STENCIL8)
        {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    }
    
    if (_isMultiSampling)
    {
        if (_msaaColorBuffer)
        {
            glDeleteRenderbuffers(1, &_msaaColorBuffer);
            _msaaColorBuffer = 0;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, _msaaFrameBuffer);
        glGenRenderbuffers(1, &_msaaColorBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _msaaColorBuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, _numSmaples, _pixelFormat, _backWidth, _backHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _msaaColorBuffer);
        if( (error=glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
        {
            NSLog(@"Failed to make complete framebuffer object 0x%X", error);
            return NO;
        }
    }
    
    if( (error=glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object 0x%X", error);
        return NO;
    }
    return YES;
}

@end
