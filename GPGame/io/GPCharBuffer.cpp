#include "stdafx.h"
#include "GPCharBuffer.h"

namespace GPEngine3D {
    CharBuffer::CharBuffer(int iMaxSize) : ui_capacity(iMaxSize), used(0), next_size(0)
    {
        c_buffer = GP_NEW char[iMaxSize+1];
    }
    
    
    CharBuffer::~CharBuffer(void)
    {
        if(c_buffer)
            GP_DELETE c_buffer;
    }
    
    bool CharBuffer::Flush(std::FILE* file)
    {
        if(!file||used == 0)
            return false;
        c_buffer[used] = '\0';
        fwrite(c_buffer,used,1,file);
        used = 0;
        next_size = 0;
        return true;
    }
    
    bool CharBuffer::Write(const char* value, std::size_t length )
    {
        if(c_buffer)
        {
            if(used+length > ui_capacity)
            {
                return false;
            }
            
            memcpy(c_buffer+used,value, length);
            used += length;
            return true;
        }
        return false;
    }
    
    
    void CharBuffer::finish(std::FILE* file)
    {
        Flush(file);
        c_buffer[used] = '\0';
        fwrite(c_buffer,1,1,file);
    }
}



