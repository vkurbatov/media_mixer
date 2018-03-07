#include "OsnovaLab/Utils/Time/TimeUtils.h"

#ifdef WIN32

#include <windows.h>

#else

#include <sys/time.h>

#endif // WIN32



//#include <chrono>

namespace OsnovaLab
{
	namespace Utils
	{
		namespace Time
		{

			time_msec_t get_time(std::tm* time, bool local = false)
			{
				time_msec_t rc = 0;
				time_t t = std::time(nullptr);
				if (time != nullptr)
				{
					*time = *std::localtime(&t);
				}

#ifdef WIN32
				SYSTEMTIME st;
				if (local)
					GetLocalTime(&st);
				else
					GetSystemTime(&st);
				rc = st.wMilliseconds;
				if (time != nullptr)
				{
					time->tm_hour = st.wHour;
					time->tm_min = st.wMinute;
					time->tm_sec = st.wSecond;
				}
#else
                timeval tv = { 0, 0};
                gettimeofday(&tv, nullptr);

                rc = tv.tv_usec / 1000;
#endif
				return rc;
			}

			time_msec_t SystemTime(std::tm* time)
			{
				return get_time(time, false);
			}

			time_msec_t LocalTime(std::tm* time)
			{
				return get_time(time, true);
			}
		}
	}
}
