#ifndef _SCREEN_LOGGER_FACTORY_H
#define _SCREEN_LOGGER_FACTORY_H

#include "OsnovaLab/Logger/AbstractLoggerFactory.h"

namespace OsnovaLab
{
	namespace Logger
	{
		class ScreenLoggerFactory : public AbstractLoggerFactory
		{
		public:
            virtual Thread::IMutex* CreateLoggerMutex() const override;
            virtual Logger::ILogger* CreateLogger(IO::IDeviceIO* deviceIO, Thread::IMutex* mutex, const std::string& title)const override;
            virtual Thread::IMutex* CreateIOMutex() const override;
            virtual IO::IDeviceIO* CreateIO(Thread::IMutex* mutex, const std::string& deviceName = "") const override;
		};
	}
}

#endif
