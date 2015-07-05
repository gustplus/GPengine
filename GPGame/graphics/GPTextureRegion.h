//
//  GPTextureRegion.h
//  GPEngine
//
//  Created by shizhan on 14-10-1.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPTextureRegion__
#define __GPEngine__GPTextureRegion__

#include "gp.h"
#include "GPRect.h"
#include "GPTexture.h"

namespace GPEngine3D {
    class TextureRegion : public Object{
    public:
        TextureRegion(TexturePtr texture, unsigned int leftX, unsigned int topY, unsigned int width, unsigned int height);
        TextureRegion(TexturePtr texture);
        virtual ~TextureRegion();
        
        void rotate(bool isRotated);
        
        unsigned int getTopY(){
            return _topY;
        }
        unsigned int getLeftX() {
            return _leftX;
        }
        unsigned int getWidth(){
            return _width;
        }
        unsigned int getHeight() {
            return _height;
        }
        
        operator float*();
        
        TexturePtr &getTexture();
        
        
    private:
        unsigned int _leftX;
        unsigned int _topY;
        unsigned int _width;
        unsigned int _height;
        
        float _v1;
        float _v2;
        float _u1;
        float _u2;
        
        bool _isRotated;  //used for texturepacker parameter
        
        TexturePtr _texRef;
    };
    
    DEFINE_SMARTPTR(TextureRegion, TextureRegionPtr);
}

#endif /* defined(__GPEngine__GPTextureRegion__) */
