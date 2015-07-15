//
//  GPOpenGLStateManager.cpp
//  GPEngine
//
//  Created by shizhan on 14-10-1.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPOpenGLStateManager.h"
namespace GPEngine3D{
    IMP_SINGLETON(OpenGLStateManager);
    
    OpenGLStateManager::OpenGLStateManager()
    :_depthReadOnly(false),
    _useBlend(false),
    _useScissor(false),
    _useStencilBuffer(false),
    _useDepthBuffer(false),
    _useTexture2D(false),
    _clearColor(NULL)
    {
        _clearColor = new float[3];
        _clearColor[0] = 0.0;
        _clearColor[1] = 0.0;
        _clearColor[2] = 0.0;
        glGetIntegerv(GL_LINE_WIDTH, &_lineWidth);
    }
    
    OpenGLStateManager::~OpenGLStateManager()
    {
        GP_DELETE_ARRAY(_clearColor);
    }
    
    void OpenGLStateManager::enableBlend(bool useBlend)
    {
        if (_useBlend != useBlend)
        {
            _useBlend = useBlend;
        }
        if (_useBlend)
        {
            glEnable(GL_BLEND);
        }else{
            glDisable(GL_BLEND);
        }
    }
    
    void OpenGLStateManager::useBlendFunc(BlendFactor srcFactor, BlendFactor dstFactor)
    {
        if (_blendSrcFactor == srcFactor && _blendDstFactor == dstFactor)
        {
            return;
        }
        _blendDstFactor = (GLint)srcFactor;
        _blendSrcFactor = (GLint)dstFactor;
        glBlendFunc(_blendSrcFactor, _blendDstFactor);
    }
    
    void OpenGLStateManager::setLineWidth(uint_32 width)
    {
        if (_lineWidth != width)
        {
            _lineWidth = width;
            glLineWidth(_lineWidth);
            
        }
    }
    
    void OpenGLStateManager::enableDepthBuffer(bool enableDepth)
    {
        if (_useDepthBuffer == enableDepth)
            return;
        _useDepthBuffer = enableDepth;
        if (_useDepthBuffer)
        {
            glEnable(GL_DEPTH_TEST);
        }else{
            glDisable(GL_DEPTH_TEST);
        }
    }
    
    void OpenGLStateManager::enableStencilBuffer(bool enable)
    {
        if (_useStencilBuffer == enable)
        {
            return;
        }
        _useStencilBuffer = enable;
        if (_useStencilBuffer)
        {
            glEnable(GL_STENCIL_TEST);
        }else{
            glDisable(GL_STENCIL_TEST);
        }
    }
    
    void OpenGLStateManager::setClearColor(float r, float g, float b)
    {
        _clearColor[0] = r;
        _clearColor[1] = g;
        _clearColor[2] = b;
        glClearColor(r, g, b, 1);
    }
    
    void OpenGLStateManager::clearScreen()
    {
        GLint mask = GL_COLOR_BUFFER_BIT;
        if (_useDepthBuffer)
        {
            mask |= GL_DEPTH_BUFFER_BIT;
        }
        if (_useStencilBuffer) {
            mask |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(mask);
    }
    
    void OpenGLStateManager::enableScissor(bool enable)
    {
        if (enable == _useScissor)
        {
            return;
        }
        _useScissor = enable;
        if (_useScissor)
        {
            glEnable(GL_SCISSOR_TEST);
        }else{
            glDisable(GL_SCISSOR_TEST);
//            glScissor(0, 0, <#GLsizei width#>, <#GLsizei height#>)
        }
    }
    
    void OpenGLStateManager::scissor(Rect &rect)
    {
        _scissorRect.x = rect.x;
        _scissorRect.y = rect.y;
        _scissorRect.width = rect.width;
        _scissorRect.height = rect.height;
        glScissor(_scissorRect.x, _scissorRect.y, _scissorRect.width, _scissorRect.height);
    }
    
    Rect &OpenGLStateManager::getScissorRect()
    {
        return _scissorRect;
    }
    
    void OpenGLStateManager::setDepthBufferReadOnly()
    {
        if (!_depthReadOnly)
        {
            _depthReadOnly = true;
            glDepthMask(false);
        }
    }
    
    void OpenGLStateManager::setDepthBufferReadWrite()
    {
        if (_depthReadOnly)
        {
            _depthReadOnly = false;
            glDepthMask(true);
        }
    }
    
    void OpenGLStateManager::resumeOptions()
    {
        glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], 1);
        if (_useBlend)
        {
            glEnable(GL_BLEND);
            glBlendFunc(_blendSrcFactor, _blendDstFactor);
        }
        if (_useDepthBuffer)
        {
            glEnable(GL_DEPTH_TEST);
        }
        if (_useStencilBuffer)
        {
            glEnable(GL_STENCIL_TEST);
        }
        if (_depthReadOnly) {
            glDepthMask(false);
        }
        glLineWidth(_lineWidth);
        if (_useScissor) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(_scissorRect.x, _scissorRect.y, _scissorRect.width, _scissorRect.height);
        }
    }
    
    void OpenGLStateManager::shutdown()
    {
        GP_DELETE(s_instance);
    }
}