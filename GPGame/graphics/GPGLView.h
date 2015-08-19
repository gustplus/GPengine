#ifndef __GPEngine__GPGLView__
#define __GPEngine__GPGLView__

#include <stdio.h>
#include "GPRect.h"
#include "GPObject.h"
#include "stdafx.h"

namespace GPEngine3D
{
    class GLView :
		public Object
    {
    public:
        GLView();
        virtual ~GLView();
        
        virtual bool isOpenGLReady() = 0;
        virtual void swapBuffer() = 0;
        virtual void resizeView() = 0;

        virtual bool initWithRect(Rect &rect, const char *name = "GPEngine") = 0;
        virtual bool initWithFullScreen(const char *name = "GPEngine") = 0;

		virtual void setViewSize(int width, int height) = 0;

        virtual void pollEvents() = 0;
        bool willWindowClose(){return _bWillWindowClose; };
        
        bool setContentScaleFactor(float scaleX, float scaleY);

        virtual void setFrameSize(int width, int height);
        
    protected:
        bool _bWillWindowClose;        

        float _scaleX;
        float _scaleY;
        
        int _width;
        int _height;        
    };
}

#endif /* defined(__GPEngine__GPGLView__) */
