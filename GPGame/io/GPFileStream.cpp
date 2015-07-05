#include "stdafx.h"
#include "GPFileStream.h"
#include "gp.h"

#pragma warning(disable:4996) 

using namespace GPEngine3D;
using std::string;

FileStream::FileStream(void) : buffer(CharBuffer(512)) ,isReady(false), validFile(nullptr), fileName(0), l_pPerPosition(0)
{
}


FileStream::~FileStream(void)
{
	buffer.Flush(validFile);
	if(validFile)
	{
		buffer.Flush(validFile);
		fclose(validFile);
	}
	if(fileName)
	{
		GP_DELETE fileName;
	}
}

bool FileStream::Open(const char* c_pFileName, bool clear)
{
	char *mode;
	if(clear)
		mode = "wb+";
	else
		mode = "rb+";
	if(validFile){
		buffer.finish(validFile);
		fclose(validFile);
	}
	validFile = fopen(c_pFileName, mode);
	if(validFile)
	{
		int strLength = strlen(c_pFileName)+1;
		if(fileName)
		{
			GP_DELETE fileName;
		}
		fileName = GP_NEW char[strLength];
		strcpy(fileName, c_pFileName);
		isReady = true;
	}
	return validFile != nullptr;
}

bool FileStream::LoadToArray(StringStream& ss)
{
	if(!isReady)
    {
		return false;
    }
    
    fseek(validFile, 0, SEEK_END);
    long len = ftell(validFile);
    fseek(validFile, 0, SEEK_SET);
	printf("str length = %ld\n", len);
	char* tmp = GP_NEW char[len+1];
	fread(tmp, sizeof(char), len, validFile);
	tmp[len] = '\0';
	ss.Init(tmp, len);
	//fclose(validFile);
	return true;
}

int FileStream::Write(const string& value)
{
	int num = 0;
	num = value.length();
	if(validFile)
	{
		Write(num);
		const char *temp = value.c_str();
		if(!buffer.Write(temp, num))
		{
			buffer.Flush(validFile);
			buffer.Write(temp, num);
		}
		//num = fwrite(value.c_str(), sizeof(char), num, validFile);
	}
	return num;
}

int FileStream::Write(const char* c_datas, int length)
{
	int num = length;
	if(validFile)
	{
		if(!buffer.Write(c_datas, num))
		{
			buffer.Flush(validFile);
			buffer.Write(c_datas, num);
		}
		//num = fwrite(c_datas, sizeof(char), length, validFile);
	}
	return num;
}

int FileStream::Write(int value)
{
	if(validFile)
	{
		int size = sizeof(int);
		char*temp = GP_NEW char[size];
		memcpy(temp, &value, size);
		if(!buffer.Write(temp, size))
		{
			buffer.Flush(validFile);
			buffer.Write(temp, size);
		}
	}

	//return fprintf_s(validFile,"%d",value);
	return 1;
}

int FileStream::Write(double value)
{
	if(validFile)
	{
		int size = sizeof(double);
		char*temp = GP_NEW char[size];
		memcpy(temp, &value, size);
		if(!buffer.Write(temp, size))
		{
			buffer.Flush(validFile);
			buffer.Write(temp, size);
		}
	}
	return 1;
}

int FileStream::Write(float value)
{
	if(validFile)
	{
		int size = sizeof(float);
		char*temp = GP_NEW char[size];
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(temp, &value, 1, size);
#endif
		if(!buffer.Write(temp, size))
		{
			buffer.Flush(validFile);
			buffer.Write(temp, size);
		}
	}
	return 1;
}

int FileStream::Write(short value)
{
	if(validFile)
	{
		int size = sizeof(short);
		char*temp = GP_NEW char[size];
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(temp, &value, 1, size);
#endif
		if(!buffer.Write(temp, size))
		{
			buffer.Flush(validFile);
			buffer.Write(temp, size);
		}
	}
	return 1;
}

int FileStream::Write(long value)
{
	if(validFile)
	{
		int size = sizeof(long);
		char*temp = GP_NEW char[size];
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(temp, &value, 1, size);
#endif
		if(!buffer.Write(temp, size))
		{
			buffer.Flush(validFile);
			buffer.Write(temp, size);
		}
	}
	return 1;
}

int FileStream::Read(std::string& value)
{
	int length;
	Read(length);
	char *a = new char[length+1];
	Read(a,length);
	a[length] = '\0';
	value = string(a);
	delete []a;
	return length;
}

int FileStream::Read(char* result, int length)
{
	int num = 0;
	if(validFile){
		num = fread(result, sizeof(char), length, validFile);
	}
	return num;
}

int FileStream::Read(int &value)
{
	if(validFile){
		int size = sizeof(int);
		char *tmp = GP_NEW char[size];
		fread(tmp, size, 1, validFile);
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(tmp, &value, 1, size);
#endif
	}
	return 1;
}

int FileStream::Read(double &value)
{
	if(validFile)
	{
		int size = sizeof(double);
		char *tmp = GP_NEW char[size];
		fread(tmp, size, 1, validFile);
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(tmp, &value, 1, size);
#endif
	}
	return 1;
}

int FileStream::Read(float &value)
{
	if(validFile)
	{
		int size = sizeof(float);
		char *tmp = GP_NEW char[size];
		fread(tmp, size, 1, validFile);
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(tmp, &value, 1, size);
#endif
	}
	return 1;
}

int FileStream::Read(short &value)
{
	if(validFile)
	{
		int size = sizeof(short);
		char *tmp = GP_NEW char[size];
		fread(tmp, size, 1, validFile);
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(tmp, &value, 1, size);
#endif
	}
	return 1;
}

int FileStream::Read(long &value)
{
	if(validFile)
	{
		int size = sizeof(long);
		char *tmp = GP_NEW char[size];
		fread(tmp, size, 1, validFile);
#ifdef PLATFORM_WIN32
		memcpy_s(temp, size, &value, size);
#else
        memccpy(tmp, &value, 1, size);
#endif
	}
	return 1;
}


bool FileStream::MoveToEnd()
{
	if(validFile)
	{
		fseek(validFile, 0L,SEEK_END);
		return true;
	}
	return false;
}

bool FileStream::MoveToBegin()
{
	if(validFile)
	{
		fseek(validFile, 0L,SEEK_SET);
		return true;
	}
	return false;
}

bool FileStream::MoveForwards(long l_diff)
{
	if(validFile)
	{
		long result = fseek(validFile, l_diff,SEEK_CUR);
		if(result == l_diff)
			return true;
	}
	return false;
}

void FileStream::savePosition()
{
	if(validFile)
	{
		l_pPerPosition = ftell(validFile);
	}
}

void FileStream::loadPosition()
{
	if(validFile && l_pPerPosition)
	{
		fseek(validFile, l_pPerPosition, SEEK_SET);
	}
}

void FileStream::Flush()
{
	if(validFile)
		buffer.Flush(validFile);
}

