#ifndef _CHARBUFFER_H_
#define _CHARBUFFER_H_

#include <cstdio>
#include "gp.h"
#include "stdafx.h"
#include <string.h>

namespace GPEngine3D{

	class CharBuffer
	{
	public:
		CharBuffer(int iMaxSize);
		~CharBuffer(void);

		bool Flush(std::FILE* file);

		bool Write(const char* value, std::size_t length );

		void finish(std::FILE* file);

	private:
		std::size_t ui_capacity;
		std::size_t used;
		std::size_t next_size;
		char* c_buffer;
	};

}

#endif