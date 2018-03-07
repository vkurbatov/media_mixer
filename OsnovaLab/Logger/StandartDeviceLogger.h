#ifndef _OL_FILE_LOGGER_H
#define _OL_FILE_LOGGER_H

#include <sstream>

#include "OsnovaLab/Logger/Logger.h"
#include "OsnovaLab/IO/DeviceIO.h"
#include "OsnovaLab/Thread/Mutex.h"

namespace OsnovaLab
{
	namespace Logger
	{
		class StandartDeviceLogger : public ILogger
		{
			static const size_t	FORMAT_STRING_BUFFER_SIZE = 2048;

			char format_string[FORMAT_STRING_BUFFER_SIZE];

			mutable Thread::IMutex* mutex_ = nullptr;
			IO::IDeviceIO* deviceIO_ = nullptr;
			

		public:

			StandartDeviceLogger(IO::IDeviceIO* deviceIO, Thread::IMutex *mutex = nullptr);
			virtual ~StandartDeviceLogger();

			//ILogger
			virtual void Print(const char* format, ...);
			virtual void PrintLn(const char* format, ...);
			virtual void PrintV(const char* format, va_list vargs);

		protected:

			virtual std::stringstream& Title(std::stringstream& sstream);
			virtual void Out(const std::stringstream& out);

			/*
			virtual const IO::IDeviceIO* GetDeviceIO() const;
			virtual const Thread::IMutex* GetMutex() const;
			*/		

		private:

			virtual void InsidePrintV(const char* prefix, const char* format, const char* suffix, va_list list);
			inline void MutexLock();
			inline void MutexUnlock();
		};
	}
}
#endif
