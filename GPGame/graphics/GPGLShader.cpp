#include "stdafx.h"
#include "GPGLShader.h"
#include <iostream>
#include "GPStringStream.h"
#include "GPFileStream.h"

namespace GPEngine3D{
    
    GLShader::GLShader(const char *fileName,ShaderType type)
    :m_type(type), _shaderID(0)
    {
        if (type != VERTEX_SHADER && type != FRAG_SHADER){
            std::cout<<"sorry, the type of shader you chose didn`t supported!"<<std::endl;
            return;
        }
        loadFromFile(fileName);
    }
    
    GLShader::~GLShader(void)
    {
        if (_shaderID > 0){
            glDeleteShader(_shaderID);
        }
    }
    
    bool GLShader::loadFromFile(const char* fileName)
    {
        FileStream file;
        StringStream arr;
        file.Open(fileName);
        file.LoadToArray(arr);
        
        const char *source = ReplaceAll(arr.GetString(), "\r\n", "\n");
        bool ret = init(source);
        if (ret){
            std::cout<<"load shader "<<fileName<<" success"<<std::endl;
            return true;
        }
        return false;
    }
    
    GLShader::operator GLuint() const{
        return _shaderID;
    }
    
    ShaderType GLShader::getType()
    {
        return m_type;
    }
    
    bool GLShader::isVaild()
    {
        return _shaderID != 0;
    }
    
    //private
    bool GLShader::init(const char *source)
    {
        _shaderID = glCreateShader(m_type);
        if (_shaderID == 0) {
            return false;
        }
        glShaderSource(_shaderID, 1, &source, NULL);
        GLint ret;
        glCompileShader(_shaderID);
        glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &ret);
        if (!ret){
            GLchar *log = (GLchar *)malloc(ret);
            glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &ret);
            glGetShaderInfoLog(_shaderID, ret, &ret, log);
            std::cout<<log<<std::endl;
            free(log);
            glDeleteShader(_shaderID);
            return false;
        }
        return true;
    }
    
    void GLShader::detachFrom(GLuint program)
    {
        if (_shaderID) {
            glDetachShader(program, _shaderID);
        }
    }
}



