//
//  GPGLViewImp_iOS.cpp
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPGLViewImp_iOS.h"
#include "GPEAGLView.h"

namespace GPEngine3D {
    GLViewImp::GLViewImp()
    {
        
    }
    GLViewImp::~GLViewImp()
    {
        
    }
    
    void GLViewImp::initWithEAGLView(void *glview)
    {
        _eaglview = glview;
        
    }
    
    bool GLViewImp::isOpenGLReady()
    {
        return _eaglview != nullptr;
    }
    
    void GLViewImp::resizeView()
    {
        if(isOpenGLReady())
        {
            GPEAGLView *view = (__bridge GPEAGLView *) _eaglview;
            [view setNeedsLayout];
        }
    }
    
    void GLViewImp::setViewSize(int width, int height)
    {
        
    }
    
    void GLViewImp::swapBuffer()
    {
        if (isOpenGLReady())
        {
            GPEAGLView *view = (__bridge GPEAGLView *) _eaglview;
            [view swapBuffer];
        }
    }
    
    
}