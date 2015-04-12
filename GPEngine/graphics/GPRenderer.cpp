#include "stdafx.h"
#include "GPRenderer.h"
#include "gp.h"

namespace GPEngine3D {
    Renderer::Renderer(PixelFormat format, char depthBits, char stencilBits, bool doubleBuffer, int width, int height):
    pFormat(format), cDepthBits(depthBits), cStencilBits(stencilBits), bDoubleBuffer(doubleBuffer), iWidth(width), iHeight(height),
    clearColor(ColorRGBA())
    {
        if(pFormat == Renderer::FM_RGB)
            color_bits = 24;
        if(pFormat == Renderer::FM_RGBA)
            color_bits = 32;
    }
    
    
    Renderer::~Renderer(void)
    {
        
    }
    
    void Renderer::resizeViewPort(int width, int height)
    {
        this->iWidth = width;
        this->iHeight = height;
    }
}