#include <exception>

#include "OsnovaLab/IO/AbstractDeviceIO.h"

namespace OsnovaLab
{
	namespace IO
	{
		AbstractDeviceIO::AbstractDeviceIO(Thread::IMutex* mutex) : mutex_(mutex)
		{

		}

		AbstractDeviceIO::~AbstractDeviceIO()
		{

		}

        const data_size_t AbstractDeviceIO::Read(p_data_t data, data_size_t count)
		{
			size_t rc = 0;

			MutexLock();

			rc = ReadIO(data, count);

			MutexUnlock();

			return rc;
		}

        const data_size_t AbstractDeviceIO::Write(const p_data_t data, data_size_t count)
		{
			size_t rc = 0;

			MutexLock();

			rc = WriteIO(data, count);

			MutexUnlock();

			return rc;
		}

		/*
		const Thread::IMutex* AbstractDeviceIO::GetMutex() const
		{
			return mutex_;
		}
		*/

		inline void AbstractDeviceIO::MutexLock()
		{
			if (mutex_ != nullptr)
				mutex_->Lock();
		}

		inline void AbstractDeviceIO::MutexUnlock()
		{
			if (mutex_ != nullptr)
				mutex_->Unlock();
		}

	}
}
