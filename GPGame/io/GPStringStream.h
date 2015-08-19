#ifndef __STRINGSTREAM_H__
#define __STRINGSTREAM_H__

#include <string>
#include "stdafx.h"

namespace GPEngine3D{

	class StringStream
	{
	public:
		StringStream(char* value = 0, int length = 0);
		~StringStream(void);

		void Init(char* value, int length);
		size_t Size();

		int Read(char* result, int length);
		int Read(std::string&);
		int Read(int &value);
		int Read(double &value);
		int Read(float &value);
		int Read(short &value);
		int Read(long &value);

		char *GetString(){return buffer;}

	private:
		char* buffer;

		int i_size;

		int position;

	};
	char *ReplaceAll(char*src, const char *old, const char *newV);
	char *Replace(char*src, const char *old, const char *newV);
}

#endif