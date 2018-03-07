//stl
#include <cstdlib>
#include <memory>

//c
#include <stdarg.h>


//osnovalab
#include "OsnovaLab/Utils/Strings/StringUtils.h"
#include "OsnovaLab/Utils/Time/TimeUtils.h"


namespace OsnovaLab
{
	namespace Utils
	{
		namespace Strings
		{
			const std::string StringFormat(const char* format, ...)
			{
				va_list vl;
				va_start(vl, format);
				return StringFormatV(format, vl);
			}

			const std::string StringFormatV(const char* format, va_list va)
			{
				char wstr[4096];
				std::vsprintf(wstr, format, va);
				return std::string(wstr);
			}

			const std::string TimeToString(std::time_t time, std::time_t m_sec)
			{
				std::tm tm_struct;
				time_t msec = Time::LocalTime(&tm_struct);

				if (time == -1)
				{
					time = std::time(nullptr);
				}

				return StringFormat(FORMAT_TIMI_WITH_MSEC, 
					tm_struct.tm_hour, 
					tm_struct.tm_mon,
					tm_struct.tm_mday,
					tm_struct.tm_hour,
					tm_struct.tm_min,
					tm_struct.tm_sec,
					msec);

			}
		}
	}
}
