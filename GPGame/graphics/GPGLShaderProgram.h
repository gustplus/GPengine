#ifndef __SHADER_PROGRAM__
#define __SHADER_PROGRAM__

#include "GPObject.h"
#include "GPGLShader.h"
#include "glfunc.h"
#include "stdafx.h"

namespace GPEngine3D{

	class GLShaderProgram : public Object
	{
	public:
		GLShaderProgram(void);
		virtual ~GLShaderProgram(void);

		bool attachShader(const GLShaderPtr &Shader);

		bool link();
        
        bool isValid();

		void printAttributes();
		void printUniforms();

		void use();
        
        operator GLuint();
        
        GLuint getProgram(){
            return _program;
        }
        
        
        int getAttribLocation(const char *name)
        {
            return glGetAttribLocation(_program, name);
        }

	private:
		GLuint _program;
        
        GLShaderPtr _vsh;
        GLShaderPtr _fsh;
		GLShaderPtr _gsh;
	};
    DEFINE_SMARTPTR(GLShaderProgram, GLShaderProgramPtr)
}

#endif