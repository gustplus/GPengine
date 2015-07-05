#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <cstdio>
#include <cstdarg>
#include <string.h>

namespace GPEngine3D{
	class Logger
	{
	public:
		
		~Logger(void);

		static void LogToFile(const char *fileName);
		static void LogToScreen();

		static void Log(const char *log, ...);

	private:
		Logger(void);

		static FILE *dstOutput;
		static char *c_fileName;
	};
}
#endif