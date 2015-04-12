//
//  GPModel.h
//  GPEngine
//
//  Created by shizhan on 14-10-5.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPModel__
#define __GPEngine__GPModel__

#include <stdio.h>
#include <vector>
#include <map>

#include "gp.h"
#include "GPVertexBuffer.h"
#include "GPElementBuffer.h"
#include "GPGLShaderProgram.h"
#include "glfunc.h"

namespace GPEngine3D {
    class Model : public Object
    {
        std::vector<VertexBufferPtr> _attribs;
        std::map<std::string, int> _locations;
        
        ElementBufferPtr _indices;
        
        DRAW_TYPE _drawType;
        
        GLShaderProgramPtr _shaderProgram;
        
        bool _isDirty;
        
#ifdef GLES3
        GLuint _VAO;
#endif
        
    public:
        Model();
        Model(GLShaderProgramPtr &);
        ~Model();
        
        void attachShaderProgram(GLShaderProgramPtr &);
        void setDrawType(DRAW_TYPE type);
        
        bool bindAttribWithVertexBuffer(const char *name, VertexBufferPtr &vertices, bool overWrite = true);
        void setIndices(ElementBufferPtr &);
        
        bool bind();
        
        void draw();
    };
}

#endif /* defined(__GPEngine__GPModel__) */
