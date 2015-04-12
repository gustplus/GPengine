//
//  GPElementBuffer.cpp
//  GPEngine
//
//  Created by shizhan on 14-10-5.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPElementBuffer.h"
#include "stdafx.h"
#include "gp.h"

US_NS_GP;

ElementBuffer::ElementBuffer(unsigned short *indice, int num, bool isDynamic)
:_num(num),
_VBO(0),
_isDirty(false),
_isDynamic(isDynamic)
{
    genElementBuffer(indice);
}

ElementBuffer::~ElementBuffer(void)
{
    if (_VBO != 0){
        glDeleteBuffers(1, &_VBO);
    }
}

int ElementBuffer::getElementNum()
{
    return _num;
}

bool ElementBuffer::genElementBuffer(const unsigned short *indices)
{
    glGenBuffers(1, &_VBO);
    if(_VBO == 0)
    {
        return false;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _num * sizeof(unsigned short), indices, _isDynamic ? VBO_DYNAMIC_TYPE : VBO_STATIC_TYPE);
#ifndef GLES3
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

    _isDirty = true;
    return true;
}

void ElementBuffer::changeElementDataAt(uint index, const unsigned short *data)
{
    if (index < _num)
    {
        this->bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * sizeof(unsigned short), sizeof(unsigned short), data);
    }else{
        printf("index is invalid at %s, %d", __FILE__, __LINE__);
    }
    
}

void ElementBuffer::changeElementDataWithRange(uint startIndex, uint length, const unsigned short *data)
{
    if (startIndex >= _num)
    {
        printf("index is invalid at %s, %d", __FILE__, __LINE__);
    }
    if (startIndex + length >= _num)
    {
        printf("length is invalid at %s, %d", __FILE__, __LINE__);
    }
    this->bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, startIndex * sizeof(unsigned short), length * sizeof(unsigned short), data);
}

void ElementBuffer::bind()
{
    if(_VBO != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBO);
    }
}