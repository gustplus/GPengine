//
//  GPGLView.h
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPGLView__
#define __GPEngine__GPGLView__

#include <stdio.h>
namespace GPEngine3D
{
    class GLView
    {
    public:
        GLView();
        virtual ~GLView();
        
        virtual bool isOpenGLReady() = 0;
        virtual void swapBuffer() = 0;
        virtual void resizeView() = 0;
        
        bool setContentScaleFactor(float scaleX, float scaleY);
        
#ifdef GP_PLATFORM_IOS
        virtual void initWithEAGLView(void *glview) = 0;
#endif
        
    protected:
        virtual void setViewSize(int width, int height) = 0;
        
    private:
        float _scaleX;
        float _scaleY;
        
        int _width;
        int _height;
        
    };
}

#endif /* defined(__GPEngine__GPGLView__) */
