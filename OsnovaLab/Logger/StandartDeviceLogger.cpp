
#include <cstdio>
#include <cstdarg>

#include "OsnovaLab/Logger/StandartDeviceLogger.h"
#include "OsnovaLab/Builder.h"

namespace OsnovaLab
{
	namespace Logger
	{
		StandartDeviceLogger::StandartDeviceLogger(IO::IDeviceIO *deviceIO, Thread::IMutex *mutex) :
			deviceIO_(deviceIO), 
			mutex_(mutex)
		{
		
		}

		StandartDeviceLogger::~StandartDeviceLogger()
		{
			try
			{
				if (deviceIO_ != nullptr)
				{
					delete deviceIO_;
				}

				if (mutex_ != nullptr)
				{
					delete mutex_;
				}
			}
			catch (std::exception &e)
			{
				//TODO
			}
		}

		void StandartDeviceLogger::Print(const char* format, ...)
		{
			va_list vl;
			va_start(vl, format);
			
			InsidePrintV(nullptr, format, nullptr, vl);

			va_end(vl);
		}

		void StandartDeviceLogger::PrintLn(const char* format, ...)
		{
			va_list vl;
			va_start(vl, format);

			InsidePrintV(nullptr, format, "\n", vl);

			va_end(vl);
		}

		void StandartDeviceLogger::PrintV(const char* format, va_list vargs)
		{
			InsidePrintV(nullptr, format, nullptr, vargs);
		}

		std::stringstream& StandartDeviceLogger::Title(std::stringstream& sstream)
		{			
			return sstream;
		}

		void StandartDeviceLogger::Out(const std::stringstream& out)
		{
			deviceIO_->Write((const IO::p_data_t)out.str().c_str(), out.str().length() + 1);
		}

		void StandartDeviceLogger::InsidePrintV(const char* prefix, const char* format, const char* suffix, va_list vargs)
		{
			std::stringstream ss;

			Title(ss);

			if (prefix != nullptr)
				ss << prefix;

			MutexLock();

            std::vsprintf(format_string, format, vargs);
			ss << format_string;

			MutexUnlock();

			if (suffix != nullptr)
				ss << suffix;

			Out(ss);
		}

		inline void StandartDeviceLogger::MutexLock()
		{
			if (mutex_ != nullptr)
				mutex_->Lock();
		}

		inline void StandartDeviceLogger::MutexUnlock()
		{
			if (mutex_ != nullptr)
				mutex_->Unlock();
		}
		
	}
}
