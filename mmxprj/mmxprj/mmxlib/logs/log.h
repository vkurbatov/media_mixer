#ifndef _MMX_LOG_H
#define _MMX_LOG_H

namespace mmx
{
	namespace logs
	{	
        enum log_level_t : int
		{
            L_TRACE,
			L_DEBUG,
			L_INFO,
			L_WARNING,
			L_ERROR,
			L_CRITICAL
        };

        int log_init(const char* filename = nullptr, log_level_t max_level = L_DEBUG, bool mt = false);
        void log(log_level_t level, const char* format, ...);
        void logT(const char* format, ...);
        void logD(const char* format, ...);
        void logI(const char* format, ...);
        void logW(const char* format, ...);
        void logE(const char* format, ...);
        void logC(const char* format, ...);
	}
}

#endif
