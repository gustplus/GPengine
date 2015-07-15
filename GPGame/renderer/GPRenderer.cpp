#include "stdafx.h"
#include "GPRenderer.h"
#include "gp.h"

namespace GPEngine3D {
    Renderer::Renderer(PixelFormat format, char depthBits, char stencilBits, bool doubleBuffer, int width, int height, int x, int y):
    pFormat(format),
    cDepthBits(depthBits),
    cStencilBits(stencilBits),
    bDoubleBuffer(doubleBuffer),
    iWidth(width),
    iHeight(height),
	iLeftX(x),
	iBottomY(y),
    colorBytes(3)
    {
		if(pFormat == PixelFormat::RGB)
            colorBytes = 3;
        if(pFormat == PixelFormat::RGBA)
            colorBytes = 4;
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