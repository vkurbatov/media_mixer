#ifndef _OL_SCREEN_LOGGER_H
#define _OL_SCREEN_LOGGER_H

#include "OsnovaLab\Logger\StandartDeviceLogger.h"
#include "OsnovaLab\Logger\ScreenLoggerFactory.h"

namespace OsnovaLab
{
	namespace Logger
	{
		class ScreenLogger : public StandartDeviceLogger
		{
			friend ScreenLoggerFactory;
			std::string title_;
			ScreenLogger(IO::IDeviceIO * deviceIO, Thread::IMutex * mutex, const std::string& title);
		public:
			virtual ~ScreenLogger();

		protected:

			virtual std::stringstream& Title(std::stringstream& s);
		};
	}
}

#endif