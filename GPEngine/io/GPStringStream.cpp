#include "stdafx.h"
#include "GPStringStream.h"
#include "gp.h"

namespace GPEngine3D {
    StringStream::StringStream(char* value, int length) : buffer(value), i_size(length)
    {
    }
    
    StringStream::~StringStream(void)
    {
        GP_DELETE buffer;
    }
    
    void StringStream::Init(char* value, int length)
    {
        if(buffer)
            GP_DELETE[] buffer;
        buffer = value;
        i_size = length;
    }
    
    int StringStream::Read(std::string& value)
    {
        int length;
        Read(length);
        char *a = new char[length+1];
        Read(a,length);
        a[length] = '\0';
        value = std::string(a);
        delete []a;
        return length;
    }
    
    
    int StringStream::Read(char* result, int length)
    {
        if((position + length) > i_size)
            return 0;
        if(buffer){
#ifdef PLATFORM_WIN32
            memcpy_s(result, length, buffer+position,length);
#else
            memcpy(result, buffer + position, length);
#endif
            position += length;
            return length;
        }
        return 0;
    }
    
    int StringStream::Read(int &value)
    {
        int size = sizeof(int);
        if((position + size) > i_size)
            return 0;
        if(buffer){
#ifdef PLATFORM_WIN32
            memcpy_s(&value, size, buffer+position, size);
#else
            memcpy(&value, buffer + position, size);
#endif
            position += size;
            return 1;
        }
        return 0;
    }
    
    int StringStream::Read(double &value)
    {
        int size = sizeof(double);
        if((position + size)>i_size)
            return 0;
        if(buffer){
#ifdef PLATFORM_WIN32
            memcpy_s(&value, size, buffer+position, size);
#else
            memcpy(&value, buffer + position, size);
#endif
            position += size;
            return 1;
        }
        return 0;
    }
    
    int StringStream::Read( float &value)
    {
        int size = sizeof(float);
        if((position + size)>i_size)
            return 0;
        if(buffer){
#ifdef PLATFORM_WIN32
            memcpy_s(&value, size, buffer+position, size);
#else
            memcpy(&value, buffer + position, size);
#endif
            position += size;
            return 1;
        }
        return 0;
    }
    
    int StringStream::Read( short &value)
    {
        int size = sizeof(short);
        if((position + size)>i_size)
            return 0;
        if(buffer){
#ifdef PLATFORM_WIN32
            memcpy_s(&value, size, buffer+position, size);
#else
            memcpy(&value, buffer + position, size);
#endif
            position += size;
            return 1;
        }
        return 0;
    }
    
    int StringStream::Read(long &value)
    {
        int size = sizeof(long);
        if((position + size)>i_size)
            return 0;
        if(buffer){
#ifdef PLATFORM_WIN32
            memcpy_s(&value, size, buffer+position, size);
#else
            memcpy(&value, buffer + position, size);
#endif
            position += size;
            return 1;
        }
        return 0;
    }
    
    size_t StringStream::Size()
    {
        return strlen(buffer);
    }
    
    
    char *GPEngine3D::Replace(char*src, const char *old, const char *newV)
    {
        int pos =0;
        int offset =0;
        int srcLen, subLen, dstLen;
        char*pRet = NULL;
        
        srcLen = strlen(src);
        subLen = strlen(old);
        dstLen = strlen(newV);
        
        char *tmp = strstr(src, old);
        if(tmp == NULL)
        {
            return src;
        }
        pRet = (char *)malloc(srcLen + dstLen - subLen +1);
        if( NULL != pRet )
        {
            pos = tmp - src;
            memcpy(pRet, src, pos);
            offset += pos;
            memcpy(pRet + offset, newV, dstLen);
            offset += dstLen;
            memcpy(pRet + offset, src + pos + subLen, srcLen - pos - subLen);
            offset += srcLen - pos - subLen;
            *(pRet + offset) ='\0';
        }
        return pRet;
    }
    
    char *GPEngine3D::ReplaceAll(char*src, const char *old, const char *newV)
    {
        char *tmp = NULL;
        int len = strlen(src)+1;
        char *result = (char *)malloc(len);
#ifdef PLATFORM_WIN32
        strcpy_s(result, len, src);
#else
        strcpy(result, src);
#endif
        while(true)
        {
            tmp = Replace(result, old, newV);
            if(tmp == result)
                return result;
            else
            {
                free(result);
                result = tmp;
            }
        }
    }
}
