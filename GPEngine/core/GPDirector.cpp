//
//  GPDirector.cpp
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPDirector.h"

namespace GPEngine3D {
    IMP_SINGLETON(Director);
    
    Director::Director()
    {
        
    }
    
    Director::~Director()
    {
        
    }
    
    void Director::mainLoop()
    {
        if(_openglView && _openglView->isOpenGLReady())
        {
            _openglView->swapBuffer();
        }
    }
    
    bool Director::setGLView(GLView *view)
    {
        _openglView = view;
        return true;
    }
    
    void init()
    {
        
    }
}