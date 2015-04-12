//
//  GPGLView.m
//  OpenGLES3
//
//  Created by zh on 14-11-3.
//  Copyright (c) 2014年 com.gp. All rights reserved.
//

#import "GPEAGLView.h"

@implementation GPEAGLView

-(id) initWithFrame:(CGRect)frame PixelFormat:(PixelFormat) pixelFormat DepthFormat:(DepthFormat) depthFormat multiSampling:(BOOL) multiSampling WithNumOfSamples:(unsigned int) num
{
    self = [super initWithFrame:frame];
    if (self)
    {
        _multiSampling = multiSampling;
        _renderLayer = (CAEAGLLayer *)self.layer;
        _renderLayer.opaque = YES;
        NSString *strPixelFormat;
        if (pixelFormat == RGBA8){
            strPixelFormat = kEAGLColorFormatRGBA8;
        }else{
            strPixelFormat = kEAGLColorFormatRGB565;
        }
        _renderLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                          strPixelFormat, kEAGLDrawablePropertyColorFormat, nil];
        
        _render = [[GPESRender alloc] initWithPixelFormat:pixelFormat DepthFormat:depthFormat multiSampling:NO WithNumOfSamples:0];
    }
    return self;
}
//override, tell the view to create the layer as CAEAGLLayer
+(Class) layerClass
{
    return [CAEAGLLayer class];
}

-(void) swapBuffer
{
    [_render.context presentRenderbuffer:GL_RENDERBUFFER];
    
    if (_multiSampling && _render.msaaFrameBuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _render.msaaFrameBuffer);
    }
}

- (void) layoutSubviews
{
    [_render resizeFrameBufferWithLayer:_renderLayer];
    glClearColor(0, 1, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    [self swapBuffer];
}

-(int) getWidth
{
    return _render.width;
}

-(int) getHeight
{
    return _render.height;
}

@end
