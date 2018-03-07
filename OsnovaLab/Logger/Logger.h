#ifndef _OL_LOGGER_H
#define _OL_LOGGER_H

#include <string>

namespace OsnovaLab
{
	namespace Logger
	{
		class ILogger
		{
		public:
			virtual void Print(const char* format, ...) = 0;
			virtual void PrintLn(const char* format, ...) = 0;
			virtual void PrintV(const char* format, va_list vargs) = 0;
		};
	}
}
#endif