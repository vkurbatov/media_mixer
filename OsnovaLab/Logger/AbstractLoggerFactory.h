#ifndef _ABSTRACT_LOGGER_FACTORY_H
#define _ABSTRACT_LOGGER_FACTORY_H

#include <string>

#include "OsnovaLab/Logger/Logger.h"
#include "OsnovaLab/Thread/Mutex.h"
#include "OsnovaLab/IO/DeviceIO.h"

namespace OsnovaLab
{
	namespace Logger
	{
		class AbstractLoggerFactory
		{
		public:
            virtual Thread::IMutex* CreateLoggerMutex() const = 0;
            virtual Logger::ILogger* CreateLogger(IO::IDeviceIO* deviceIO, Thread::IMutex* mutex, const std::string& title) const = 0;
            virtual Thread::IMutex* CreateIOMutex() const = 0;
            virtual IO::IDeviceIO* CreateIO(Thread::IMutex* mutex, const std::string& deviceName = "") const = 0;
		};
	}
}

#endif
