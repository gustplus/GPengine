//
//  GPGLViewImp_iOS.h
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPGLViewImp_iOS__
#define __GPEngine__GPGLViewImp_iOS__

#include <stdio.h>
#include "GPGLView.h"

namespace GPEngine3D
{
    class GLViewImp : public GLView
    {
    public:
        GLViewImp();
        virtual ~GLViewImp();
        
        virtual void initWithEAGLView(void *glview);
        
        virtual bool isOpenGLReady();
        
        virtual void swapBuffer();
        
        virtual void resizeView();
        
    protected:
        virtual void setViewSize(int width, int height);
        
    private:
        void *_eaglview;
    };
}

#endif /* defined(__GPEngine__GPGLViewImp_iOS__) */
