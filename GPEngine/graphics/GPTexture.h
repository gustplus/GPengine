#pragma once
#include "gpobject.h"
#include "glfunc.h"
#include "gpimage.h"
#include "gp.h"
#include "GPSmartPtr.h"

namespace GPEngine3D
{
    enum WrapType{
        CLIMP,
        REPEAT,
        REPRAT_MIRROR
    };
    
    enum TextureHint{
        FAST,
        NICE
    };
    
	class Texture :
		public Object
	{
	public:
        Texture(const Texture &);
		Texture(const char *fileName, GLenum type = GL_TEXTURE_2D);
        Texture(const ImagePtr &image, GLenum type = GL_TEXTURE_2D);
		virtual ~Texture(void);

		bool init(byte *bytes);
        
        int getWidth(){
            return width;
        }
        
        int getHeight(){
            return height;
        }
        
        void setWrapType(WrapType type);

		void bind();
        
        void dispose();

        void setHint(TextureHint hint);

	private:
		GLenum m_type;
		const char * m_fileName;
		GLuint textureID;
        
        int width;
        int height;
        
        TextureHint hint;
	};
    
    DEFINE_SMARTPTR(Texture, TexturePtr);
}
