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
#include "GPScreen.h"

namespace GPEngine3D {
    class Director
    {
    public:
        DEFINE_SINGLETON(Director);
        
        ~Director();

        void runWithScene(Screen *scene);
        
		bool setGLView(GLView *view);
        
        inline SmartPtr<GLView> getGLView(){ return _openglView; };
        
        void mainLoop(float deltaTime);
        
    private:
        SmartPtr<GLView> _openglView;

        SmartPtr<Screen> _runningScene;

		float _timeLast;
		int _frameLast;
    };
}

#endif /* defined(__GPEngine__GPDirector__) */
