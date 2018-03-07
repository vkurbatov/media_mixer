#ifndef _OL_STRING_UTILS_H
#define _OL_STRING_UTILS_H

#include <string>
#include <iostream>
#include <ctime>
#include <stdarg.h>


namespace OsnovaLab
{
	namespace Utils
	{
		namespace Strings
		{
			static const char* FORMAT_TIMI_WITH_MSEC = "%.2d.%.2d.%.2d %.2d:%.2d:%.2d.%.3d";

			const std::string StringFormat(const char* format, ...);
			const std::string StringFormatV(const char* format, va_list va);
			const std::string TimeToString(std::time_t time = -1, std::time_t m_sec = 0);
		}
	}
}

#endif
