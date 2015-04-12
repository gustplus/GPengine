//
//  GPModel.cpp
//  GPEngine
//
//  Created by shizhan on 14-10-5.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPModel.h"

namespace GPEngine3D {
    Model::Model()
    :_shaderProgram(NULL),
    _indices(NULL),
    _attribs(std::vector<VertexBufferPtr>(3)),
    _drawType(TRIANGLE),
    _isDirty(false)
    {
        
    }
    
    Model::Model(GLShaderProgramPtr &program)
    :_shaderProgram(program),
    _indices(NULL),
    _attribs(std::vector<VertexBufferPtr>(3)),
    _drawType(TRIANGLE),
    _isDirty(false)
    {
        
    }
    
    Model::~Model()
    {
        
    }
    
    void Model::attachShaderProgram(GLShaderProgramPtr &program)
    {
        _shaderProgram = program;
    }
    
    void Model::setDrawType(DRAW_TYPE type)
    {
        _drawType = type;
    }
    
    bool Model::bindAttribWithVertexBuffer(const char *name, VertexBufferPtr &vertices, bool overWrite)
    {
        int location;
        std::map<std::string, int>::iterator it = _locations.find(name);
        if (it != _locations.end())
        {
            location = it->second;
        }else{
            location = _shaderProgram->getAttribLocation(name);
            if (location < 1)
            {
                printf("can't find attrib : %s", name);
                return false;
            }
            _locations.insert(std::map<std::string, int>::value_type(name, location));
        }
        vertices->bindToAttribLocation(location);
        _attribs.push_back(vertices);
        _isDirty = true;
        return true;
    }
    
    void Model::setIndices(ElementBufferPtr &elementBuffer)
    {
        _indices = elementBuffer;
    }
    
    bool Model::bind()
    {
        if (_isDirty) {
#ifdef GLES3
            glGenVertexArrays(1, &_VAO);
            if (_VAO < 1) {
                printf("can't gen vao");
                return false;
            }
            glBindVertexArray(_VAO);
#endif
            std::vector<VertexBufferPtr>::iterator it = _attribs.begin();
            for (; it != _attribs.end(); ++it) {
                if (!(*it)->bind()){
                    printf("location %d bind failed", (*it)->getBindedLocation());
                }
            }
            if (_indices)
            {
                _indices->bind();
            }
            _isDirty = false;
            return true;
        }else{
#ifdef GLES3
            glBindVertexArray(_VAO);
#else
            std::vector<VertexBufferPtr>::iterator it = _attribs.begin();
            for (; it != _attribs.end(); ++it) {
                if (!(*it)->bind()){
                    printf("location %d bind failed", (*it)->getBindedLocation());
                }
            }
            if (_indices)
            {
                _indices->bind();
            }
            return true;
#endif
        }
        return false;
    }
    
    void Model::draw()
    {
        if (_indices) {
            glDrawElements(_drawType, _indices->getElementNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET_BYTE(0));
        }else{
            glDrawArrays(_drawType, 0, (*_attribs.begin())->getVerticeNum());
        }
    }
}