#ifndef _OL_TITLE_TIMESTAMP_LOGGER_H
#define _OL_TITLE_TIMESTAMP_LOGGER_H

#include "OsnovaLab/Logger/StandartDeviceLogger.h"


namespace OsnovaLab
{
	namespace Logger
	{
		class FileLoggerFactory;
		class ScreenLoggerFactory;

		class TitleTimeStampLogger : public StandartDeviceLogger
		{
			friend ScreenLoggerFactory;
			friend FileLoggerFactory;
			std::string title_;
			TitleTimeStampLogger(IO::IDeviceIO * deviceIO, Thread::IMutex * mutex, const std::string& title);
		public:
			virtual ~TitleTimeStampLogger();

		protected:

			virtual std::stringstream& Title(std::stringstream& sstream);
		};
	}
}

#endif
