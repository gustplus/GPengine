//
//  GPGLView.cpp
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPGLView.h"
#include "glfunc.h"

namespace GPEngine3D {
    
    GLView::GLView():
    _scaleX(1),
    _scaleY(1),
    _width(0),
    _height(0),
	_bWillWindowClose(false)
    {
    }
    
    GLView::~GLView()
    {
        
    }
    
    bool GLView::setContentScaleFactor(float scaleX, float scaleY)
    {
        if (scaleX != _scaleX || scaleY != _scaleY)
        {
            _scaleX = scaleX;
            _scaleY = scaleX;
            
            int width = _scaleX * _width;
            int height = _scaleY * _height;
            
            setViewSize(width, height);
        }
        
        return true;
    }

    void GLView::setViewSize(int width, int height)
    {
        _width = width;
        _height = height;
    }

	void GLView::setFrameSize(int width, int height)
	{

	}
}