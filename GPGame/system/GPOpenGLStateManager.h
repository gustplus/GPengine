//
//  GPOpenGLStateManager.h
//  GPEngine
//
//  Created by shizhan on 14-10-1.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPOpenGLStateManager__
#define __GPEngine__GPOpenGLStateManager__

#include <stdio.h>
#include "gp.h"
#include "glfunc.h"
#include "GPRect.h"

namespace GPEngine3D {
    class OpenGLStateManager{
    public:
        void enableBlend(bool useBlend);
        
        void useBlendFunc(BlendFactor srcFactor, BlendFactor dstFactor);
        
        void setLineWidth(uint width);
        
        void enableDepthBuffer(bool enableDepth);
        
        void enableStencilBuffer(bool enable);
        
        void setClearColor(float r, float g, float b);
        
        void clearScreen();
        
        void enableScissor(bool enable);
        
        void scissor(Rect &rect);
        
        Rect &getScissorRect();
        
        void setDepthBufferReadOnly();
        
        void setDepthBufferReadWrite();
        
        void resumeOptions();
        
        void shutdown();
        
        DEFINE_SINGLETON(OpenGLStateManager);
        
        ~OpenGLStateManager();
        
        GLint _lineWidth;
        GLint _blendSrcFactor;
        GLint _blendDstFactor;
        
        float* _clearColor;
        
        Rect _scissorRect;
        
        bool _useTexture2D;
        bool _useBlend;
        bool _useDepthBuffer;
        bool _useStencilBuffer;
        bool _useScissor;
        bool _depthReadOnly;
    };
}

#endif /* defined(__GPEngine__GPOpenGLStateManager__) */
