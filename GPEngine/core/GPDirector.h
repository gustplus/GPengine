//
//  GPDirector.h
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPDirector__
#define __GPEngine__GPDirector__

#include <stdio.h>
#include "GPObject.h"
#include "gp.h"
#include "GPGLView.h"

namespace GPEngine3D {
    class Director
    {
    public:
        DEFINE_SINGLETON(Director);
        
        ~Director();
        
        void init();
        
        bool setGLView(GLView *view);
        
        inline GLView *getGLView(){ return _openglView; };
        
        void mainLoop();
        
    private:
        GLView * _openglView;
    };
}

#endif /* defined(__GPEngine__GPDirector__) */
