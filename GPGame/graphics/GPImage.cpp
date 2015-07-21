//
//  GPImage.cpp
//  GPEngine
//
//  Created by shizhan on 14-9-14.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPImage.h"
#include "gp.h"
#include "GPFIleUtil.h"

using std::string;

namespace GPEngine3D {
    Image::Image():
    iWidth(0),
    iHeight(0),
    bImageData(nullptr)
    {
        
    }
    
    Image::~Image()
    {
        GP_SAFE_DELETE_ARRAY(bImageData);
    }
    
    void Image::initWithFile(const string &fileName)
    {
        string path = FileUtil::getInstance()->getFullPathOfFile(fileName);
        //todo init byte array with filename
//        int width = ;
//        int height = ;
        
    }
    
    void Image::initWithBytes(byte *bytes, int width, int height)
    {
        this->iWidth = width;
        this->iHeight = height;
        this->bImageData = bytes;
    }
    
    byte *const Image::getData() const
    {
        return bImageData;
    }
}