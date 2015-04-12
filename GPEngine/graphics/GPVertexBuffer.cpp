#include "stdafx.h"
#include "GPVertexBuffer.h"
#include "gp.h"

namespace GPEngine3D {
    VertexBuffer::GPEngine3DVertexBuffer(float *vertices, int num_vertices, DATATYPE data_type, int nums_per_vertex, bool isDynamic)
    :_num(num_vertices),
    _VAO(0),
    _VBO(0),
    _isDirty(false),
    _isDynamic(isDynamic),
    _nums_per_vertex(nums_per_vertex),
    _isMapingData(false),
    _byte_per_data_cell((byte) data_type)
    {
        genArrayBuffer(vertices);
    }
    
    VertexBuffer::~VertexBuffer(void)
    {
#ifdef GLES3
        if(_VAO != 0)
        {
            glDeleteVertexArrays(1, &_VAO);
        }
#endif
        if (_VBO != 0){
            glDeleteBuffers(1, &_VBO);
        }
    }
    
    int VertexBuffer::getVerticeNum()
    {
        return _num;
    }
    
    bool VertexBuffer::genArrayBuffer(const float *vertices)
    {
#ifdef GLES3
        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);
#endif
        glGenBuffers(1, &_VBO);
        if(_VBO == 0)
        {
            return false;
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _num * _byte_per_data_cell, vertices, _isDynamic ? VBO_DYNAMIC_TYPE : VBO_STATIC_TYPE);
#ifndef GLES3
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
        
#ifdef GLES3
        glBindVertexArray(0);
#endif
        _isDirty = true;
        return true;
    }
    
    void VertexBuffer::changeVertexDataAt(uint index, const float *data)
    {
        if (index < _num)
        {
            this->bind();
            glBufferSubData(GL_ARRAY_BUFFER, index * _byte_per_data_cell * _byte_per_data_cell, _byte_per_data_cell, data);
        }else{
            printf("index is invalid at %s, %d", __FILE__, __LINE__);
        }
        
    }
    
    void VertexBuffer::changeVertexDataWithRange(uint startIndex, uint length, const float *data)
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
        glBufferSubData(GL_ARRAY_BUFFER, startIndex * _byte_per_data_cell * _byte_per_data_cell, length * _byte_per_data_cell, data);
    }
    
    void VertexBuffer::bind()
    {
#ifdef GLES3
        if (_VAO != 0){
            glBindVertexArray(_VAO);
        }
#else
        if(_VBO != 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        }
#endif
    }
    
    float *VertexBuffer::mapData(uint startIndex, int length, bool isRead)
    {
        if (_isMapingData)
        {
            printf("data has been mapped, please unmap it first");
            return NULL;
        }
        return (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, startIndex * nums_per_vertex * _byte_per_data_cell, length * nums_per_vertex, isRead ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT);
    }
    
    bool VertexBuffer::unmapData()
    {
        if (!_isMapingData)
        {
            printf("you hasn't map the data");
            return false;
        }
        return glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    

}
