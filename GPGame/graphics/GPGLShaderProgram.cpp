#include "stdafx.h"
#include "GPGLShaderProgram.h"
#include "GPStringStream.h"
#include <iostream>


using namespace GPEngine3D;
using std::cout;
using std::endl;

GLShaderProgram::GLShaderProgram(void)
:_program(0),
_fsh(NULL),
_vsh(NULL)
{
}

GLShaderProgram::~GLShaderProgram(void)
{
	if(_program)
	{
        printf("delete program");
		glDeleteProgram(_program);
	}
}

bool GLShaderProgram::attachShader(const GLShaderPtr &shader)
{
	if(_program > 0 && _vsh && _fsh)
    {
		glDeleteProgram(_program);
        _program = 0;
    }
    if (_program == 0)
    {
        _program = glCreateProgram();
    }
	if(_program <= 0)
	{
		cout<<"can't create program"<<endl;
        return false;
	}
    glAttachShader(_program, shader->getShader());
    
    if (shader->getType() == ShaderType::VERTEX_SHADER)
    {
        _vsh = shader;
    }
    if (shader->getType() == ShaderType::FRAG_SHADER)
    {
        _fsh = shader;
    }
	return true;
}


bool GLShaderProgram::link()
{
    if(_program<=0)
    {
        return false;
    }
    glLinkProgram(_program);
    
    GLint result;
    glGetProgramiv(_program, GL_LINK_STATUS, &result);
    if(!result)
    {
        char *log = NULL;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &result);
        log = (GLchar *)malloc(result * sizeof(GLchar));
        glGetProgramInfoLog(_program, result, &result, log);
        cout<<"Link Program "<<log<<endl;
        glDeleteProgram(_program);
        free(log);
        return false;
    }else{
        _fsh->detachFrom(_program);
        _vsh->detachFrom(_program);
        
        cout<<"link ok"<<endl;
        return true;
    }
}

bool GLShaderProgram::isValid()
{
    int len;
    glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        char *log = (char *)malloc(len);
        glGetProgramInfoLog(_program, len, &len, log);
        cout<<"program valid info: "<<log<<endl;
        free(log);
    }
    GLint status;
    glGetProgramiv(_program, GL_VALIDATE_STATUS, &status);
    if (!status)
    {
        return false;
    }
    return true;
}


void GLShaderProgram::printAttributes()
{
	GLint num = 0;
	GLint maxLength = 0;
	glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &num);
	glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

	GLint writenLength;
	GLint size;
	GLenum type;
	GLchar *name = (GLchar *)malloc(maxLength);
    if(NULL == name)
    {
        printf("allocation error! in %s at %d\n", __FILE__, __LINE__);
        return;
    }
	printf("%d Attributes\n", num);
	for(int i = 0; i<num; ++i)
	{
		glGetActiveAttrib(_program, i, maxLength, &writenLength, &size, &type, name);
		GLint location = glGetAttribLocation(_program, name);
		printf("name = %s, location = %d\n", name, location);
	}
	free(name);
}

void GLShaderProgram::printUniforms()
{
	GLint num;
	GLint maxLength;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &num);
	glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

	GLint writenLength;
	GLint size;
	GLenum type;
	GLchar *name = (GLchar *)malloc(maxLength);
	if(NULL == name)
	{
		printf("allocation error! in %s at %d\n", __FILE__, __LINE__);
		return;
	}
	printf("%d Uniforms\n", num);
	for(int i = 0; i<num; ++i)
	{
		glGetActiveUniform(_program, i, maxLength, &writenLength, &size, &type, name);
		GLint location = glGetUniformLocation(_program, name);
		printf("name = %s, location = %d\n", name, location);
	}
	free(name);
}

void GLShaderProgram::use()
{
	if(_program > 0)
    {
		glUseProgram(_program);
    }
}

GLShaderProgram::operator GLuint()
{
    return _program;
}
