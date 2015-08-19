#pragma once
#include <cstdio>
#include <string>
#include "GPCharBuffer.h"
#include "GPStringStream.h"
#include "stdafx.h"

namespace GPEngine3D{
	class FileStream
	{
	public:
		FileStream(void);
		~FileStream(void);

		bool Open(const char *c_pFileName, bool clear = false);

		bool LoadToArray(StringStream &ss);

		int Read(char* result, int length);
		int Read(std::string&);
		int Read(int &value);
		int Read(double &value);
		int Read(float &value);
		int Read(short &value);
		int Read(long &value);

		int Write(const char *c_datas, int length);
		int Write(const std::string& value);
		int Write(int value);
		int Write(double value);
		int Write(float value);
		int Write(short value);
		int Write(long value);


		bool MoveToEnd();

		bool MoveToBegin();

		bool MoveForwards(long l_diff);

		void savePosition();

		void loadPosition();

		void Flush();


	private:	
		std::FILE* validFile;
		char *fileName;
		bool isReady;
		CharBuffer buffer;
		long l_pPerPosition;	
	};
}