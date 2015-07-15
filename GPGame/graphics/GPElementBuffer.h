//
//  GPElementBuffer.h
//  GPEngine
//
//  Created by shizhan on 14-10-5.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPElementBuffer__
#define __GPEngine__GPElementBuffer__

#include "gp.h"
#include "glfunc.h"
#include "types.h"
/*
 *you can use VertexBuffer to store vertex data, include vertex position, colors, and user defined datas
 *
 */

namespace GPEngine3D
{
    class ElementBuffer :
    public Object
    {
    public:
        ElementBuffer(unsigned short *indice, int num, bool isDynamic = false);
        virtual ~ElementBuffer(void);
        
        operator float*();
        
        int getElementNum();
        
        void changeElementDataAt(uint_32 index, const unsigned short *data);
        
        void changeElementDataWithRange(uint_32 startIndex, uint_32 length, const unsigned short *data);
        
        bool genElementBuffer(const unsigned short *indice);
        
        void bind();
        
    private:
        uint_32 _num;
        GLuint _VBO;

        bool _isDirty;
        bool _isDynamic;
    };
    DEFINE_SMARTPTR(ElementBuffer, ElementBufferPtr);
}


#endif /* defined(__GPEngine__GPElementBuffer__) */
