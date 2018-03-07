#include "OsnovaLab/Logger/ScreenLoggerFactory.h"
#include "OsnovaLab/Logger/TitleTimeStampLogger.h"
#include "OsnovaLab/Thread/StdMutex.h"
#include "OsnovaLab/IO/ScreenIO.h"

namespace OsnovaLab
{
	namespace Logger
	{

        Thread::IMutex* ScreenLoggerFactory::CreateLoggerMutex() const
		{
			return new Thread::StdMutex();
		}

        Logger::ILogger* ScreenLoggerFactory::CreateLogger(IO::IDeviceIO* deviceIO, Thread::IMutex* mutex, const std::string& title) const
		{
			return new TitleTimeStampLogger(deviceIO, mutex, title);
		}

        Thread::IMutex* ScreenLoggerFactory::CreateIOMutex() const
		{
			static Thread::StdMutex screen_io_mutex;
			return new Thread::StdMutex(screen_io_mutex);
		}

        IO::IDeviceIO* ScreenLoggerFactory::CreateIO(Thread::IMutex* mutex, const std::string& deviceName) const
		{
            return new IO::ScreenIO(mutex);
            //return nullptr;
		}
	}
}
