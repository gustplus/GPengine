#include "stdafx.h"
#include "GPTexture.h"

namespace GPEngine3D {
    Texture::Texture(const char *fileName, GLenum type) : m_fileName(fileName), m_type(type), textureID(-1), width(0), height(0)
    {
        //todo
        //load image here
        hint = NICE;
    }
    
    Texture::Texture(const ImagePtr &image, GLenum type) : m_type(type), textureID(-1)
    {
        //todo
        //load image here
        byte *imageData = image->getData();
        init(imageData);
        hint = NICE;
    }
    
    Texture::Texture(const Texture &tex)
    {
        this->hint = tex.hint;
        this->m_fileName = tex.m_fileName;
        this->m_type = tex.m_type;
        this->width = tex.width;
        this->height = tex.height;
        this->textureID = tex.textureID;
    }
    
    Texture::~Texture(void)
    {
        
    }
    
    bool Texture::init(byte *bytes)
    {
        glGenTextures(1, &textureID);
        glBindTexture(m_type, textureID);
        if(GL_TEXTURE_2D == m_type)
            glTexImage2D(m_type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
            ;
        glTexParameterf(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        
        glTexParameterf(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        if (hint == NICE){
            glGenerateMipmap(m_type);
        }
        
        glBindTexture(m_type, 0);
        return true;
    }
    
    void Texture::setWrapType(WrapType type)
    {
        switch (type) {
            case CLIMP:
                glTexParameterf(m_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameterf(m_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
            case REPEAT:
                glTexParameterf(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameterf(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            case REPRAT_MIRROR:
                glTexParameterf(m_type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameterf(m_type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
            default:
                break;
        }
    }
    
    void Texture::bind()
    {
        if (textureID != -1){
            glBindTexture(m_type, textureID);
        }
    }
    
    void Texture::dispose()
    {
        if (textureID != -1) {
            glDeleteTextures(1, &textureID);
        }
    }
    
    void Texture::setHint(TextureHint hint)
    {
        this->hint = hint;
    }

}



