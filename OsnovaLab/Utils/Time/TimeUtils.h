#ifndef _OL_TIME_UTILS_H
#define _OL_TIME_UTILS_H

#include <ctime>

namespace OsnovaLab
{
	namespace Utils
	{
		namespace Time
		{
			using time_msec_t = time_t;

			time_msec_t SystemTime(std::tm* time);
			time_msec_t LocalTime(std::tm* time);		
		}
	}
}

#endif
