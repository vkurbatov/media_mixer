#ifndef _FILE_LOGGER_FACTORY_H
#define _FILE_LOGGER_FACTORY_H


#include <fstream>
#include "OsnovaLab/Logger/AbstractLoggerFactory.h"

namespace OsnovaLab
{
	namespace Logger
	{
		class FileLoggerFactory : public AbstractLoggerFactory
		{
			static const std::ios_base::openmode filemode_ = std::ios_base::app | std::ios_base::out | std::ios_base::in;
		public:
            virtual Thread::IMutex* CreateLoggerMutex() const override;
            virtual Logger::ILogger* CreateLogger(IO::IDeviceIO* deviceIO, Thread::IMutex* mutex, const std::string& title) const override;
            virtual Thread::IMutex* CreateIOMutex() const override;
            virtual IO::IDeviceIO* CreateIO(Thread::IMutex* mutex, const std::string& deviceName = "") const override;
		};
	}
}

#endif
