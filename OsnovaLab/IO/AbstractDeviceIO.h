#ifndef _OL_ABSTRACT_DEVICE_IO_H
#define _OL_ABSTRACT_DEVICE_IO_H

#include "OsnovaLab/IO/DeviceIO.h"
#include "OsnovaLab/Thread/Mutex.h"

namespace OsnovaLab
{
	namespace IO
	{
		class AbstractDeviceIO : public IDeviceIO
		{
			mutable Thread::IMutex *mutex_;

		public:
			AbstractDeviceIO(Thread::IMutex* mutex);
			virtual ~AbstractDeviceIO();
            virtual const data_size_t Read(p_data_t data, data_size_t size);
            virtual const data_size_t Write(const p_data_t data, data_size_t size);

		protected:

            virtual const data_size_t ReadIO(p_data_t data, data_size_t size) = 0;
            virtual const data_size_t WriteIO(const p_data_t data, data_size_t size) = 0;

			//virtual const Thread::IMutex* GetMutex() const;
				

		private:
			inline void MutexLock();
			inline void MutexUnlock();
		};
	}
}


#endif
