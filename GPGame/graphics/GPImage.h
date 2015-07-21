//
//  GPImage.h
//  GPEngine
//
//  Created by shizhan on 14-9-14.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPImage__
#define __GPEngine__GPImage__

#include <iostream>
#include "gp.h"
#include "GPSmartPtr.h"
#include "types.h"

namespace GPEngine3D{
    enum ImageFormat{
        IMAGE_FORMAT_RGBA
    };
    class Image:public Object{
    public:
        Image();
        ~Image();
        
        void initWithFile(const std::string &fileName);
        
        void initWithBytes(byte *bytes, int width, int height);

        int getWidth() const
        {
            return iWidth;
        }

        int getHeight() const
        {
            return iHeight;
        }
        
        byte *const getData() const;
        
    private:
        byte *bImageData;
        
        int iWidth;
        int iHeight;
        
        ImageFormat format;
        
    };
    
    DEFINE_SMARTPTR(Image, ImagePtr);
}

#endif /* defined(__GPEngine__GPImage__) */
