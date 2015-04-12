#include "stdafx.h"
#include "GPLogger.h"
#include "gp.h"

US_NS_GP;

FILE *Logger::dstOutput = stdout;
char *Logger::c_fileName = NULL;
Logger::Logger(void)
{

}


Logger::~Logger(void)
{
	if(NULL != dstOutput)
	{
		fclose(dstOutput);
	}
}

void Logger::LogToFile(const char *fileName)
{
	if(NULL == c_fileName){
		if(strcmp(fileName, c_fileName))
		{
			if(NULL != dstOutput){
				fclose(dstOutput);
				GP_DELETE(c_fileName);
				int len = strlen(fileName) + 1;
				c_fileName = GP_NEW char[len];
#ifdef PLATFORM_WIN32
                strcpy_s(c_fileName, len, fileName);
#else
                strcpy(c_fileName, fileName);
#endif
			}
			//dstOutput = fopen_s(fileName, "w+");
            
#ifdef PLATFORM_WIN32
            errno_t err = fopen_s(&dstOutput, c_fileName, "w+");
#else
//            errno_t err = fopen(&dstOutput, c_fileName, "w+");
#endif
			if(NULL == dstOutput)
			{
				printf("error");
			}
		}

	}
}

void Logger::LogToScreen()
{
	dstOutput = stdout;
}

void Logger::Log(const char *log, ...)
{
	va_list args;
	va_start(args, log);
//	fprintf(dstOutput, args); 
}
