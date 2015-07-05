#ifndef _GLSHADER_
#define _GLSHADER_

#include "gp.h"
#include "glfunc.h"
#include "GPObject.h"
#include <iostream>

namespace GPEngine3D{

	class GLShader :
		public Object
	{
        friend class GLShaderProgram;
	public:
        GLShader(const char *fileName, ShaderType type);
		virtual ~GLShader(void);

		bool loadFromFile(const char* fileName);
        
        ShaderType getType();
        
        bool isVaild();
        
        operator GLuint() const;
        
        GLuint getShader() const
        {
            return _shaderID;
        }
	private:
        bool init(const char *source);
        
        void detachFrom(GLuint program);
        
        GLuint _shaderID;
        
        const ShaderType m_type;
	};
    
    DEFINE_SMARTPTR(GLShader, GLShaderPtr);
}
#endif