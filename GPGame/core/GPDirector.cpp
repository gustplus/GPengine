//
//  GPDirector.cpp
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014 ___GUSTPLUS___. All rights reserved.
//

#include "GPDirector.h"
#include "GPkeyHandler.h"

namespace GPEngine3D {
    IMP_SINGLETON(Director);
    
    Director::Director():
		_timeLast(0),
		_frameLast(0)
    {
        
    }
    
    Director::~Director()
    {
        
    }

    void Director::runWithScene(Screen * scene)
    {
        _runningScene = SmartPtr<Screen>(scene);
    }
    
    void Director::mainLoop(float deltaTime)
    {
        if(_openglView && _openglView->isOpenGLReady())
        {
            if(_runningScene)
            {
				_timeLast += deltaTime;
				++_frameLast;
				if(_timeLast >= 1.0f){
					printf("FPS:%d\n", _frameLast);
					_timeLast -= 1;
					_frameLast = 0;
				}
                _runningScene->update(deltaTime);
                _runningScene->present();
                KeyHandler::getInstance()->rollDir = 0;
                _openglView->swapBuffer();
            }
            
        }
    }
    
    bool Director::setGLView(GLView *view)
    {
        _openglView = view;
        return true;
    }
    
}