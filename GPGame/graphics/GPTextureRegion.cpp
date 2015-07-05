//
//  GPTextureRegion.cpp
//  GPEngine
//
//  Created by shizhan on 14-10-1.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPTextureRegion.h"

namespace GPEngine3D {
    TextureRegion::TextureRegion(TexturePtr texture, unsigned int leftX, unsigned int topY, unsigned int width, unsigned int height)
    :_leftX(leftX),
    _topY(topY),
    _width(width),
    _height(height),
    _isRotated(false),
    _texRef(nullptr)
    {
        _texRef = texture;
        int tWidth = texture->getWidth();
        int tHeight = texture->getHeight();
        
        this->_u1 = (float)leftX / tWidth;
        this->_u2 = (float)(leftX + _width) / tWidth;
        this->_v1 = (float)topY / tHeight;
        this->_v2 = (float)(topY + _height) / tHeight;
    }
    
    TextureRegion::TextureRegion(TexturePtr texture)
    :_leftX(0),
    _topY(0),
    _texRef(texture),
    _width(texture->getWidth()),
    _height(texture->getHeight()),
    _isRotated(false)
    {        
        this->_u1 = 0.0;
        this->_u2 = 1.0;
        this->_v1 = 0.0;
        this->_v2 = 1.0;
    }
    
    TextureRegion::~TextureRegion()
    {
        
    }
    
    void TextureRegion::rotate(bool isRotated)
    {
        if (_isRotated != isRotated){
            this->_width += this->_height;
            this->_height = this->_width - this->_height;
            this->_width -= this->_height;
        }
        this->_isRotated = isRotated;
    }
    
    TextureRegion::operator float*()
    {
		float *ret = new float[8];
        //must delete after use
        if (_isRotated)
        {
			ret[0] = _u1;
			ret[1] = _v2;
			ret[2] = _u2;
			ret[3] = _v2;
			ret[4] = _u2;
			ret[5] = _v1;
			ret[6] = _u1;
			ret[7] = _v1;
        }else{
			ret[0] = _u1;
			ret[1] = _v1;
			ret[2] = _u2;
			ret[3] = _v1;
			ret[4] = _u1;
			ret[5] = _v2;
			ret[6] = _u2;
			ret[7] = _v2;
        }
		return ret;
    }
    
    TexturePtr &TextureRegion::getTexture()
    {
        return _texRef;
    }
}