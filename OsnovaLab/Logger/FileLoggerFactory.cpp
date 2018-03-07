#include "OsnovaLab/Logger/FileLoggerFactory.h"
#include "OsnovaLab/Logger/TitleTimeStampLogger.h"
#include "OsnovaLab/Thread/StdMutex.h"
#include "OsnovaLab/IO/FileIO.h"

namespace OsnovaLab
{
	namespace Logger
	{

        Thread::IMutex* FileLoggerFactory::CreateLoggerMutex() const
		{
			return new Thread::StdMutex();
		}

        Logger::ILogger* FileLoggerFactory::CreateLogger(IO::IDeviceIO* deviceIO, Thread::IMutex* mutex, const std::string& title) const
		{
			return new TitleTimeStampLogger(deviceIO, mutex, title);
		}

        Thread::IMutex* FileLoggerFactory::CreateIOMutex() const
		{
			static Thread::StdMutex screen_io_mutex;
			return new Thread::StdMutex(screen_io_mutex);
		}

        IO::IDeviceIO* FileLoggerFactory::CreateIO(Thread::IMutex* mutex, const std::string& deviceName) const
		{
            //return nullptr;
            return new IO::FileIO(deviceName, filemode_, mutex);
		}
	}
}
