#ifndef __GPEngine__GPDirector__
#define __GPEngine__GPDirector__

#include <stdio.h>
#include "GPObject.h"
#include "gp.h"
#include "GPGLView.h"
#include "GPScreen.h"
#include "stdafx.h"

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
