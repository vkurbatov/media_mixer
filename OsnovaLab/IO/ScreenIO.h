#ifndef _OL_SCREEN_IO_H
#define _OL_SCREEN_IO_H

#include "OsnovaLab/IO/AbstractDeviceIO.h"

namespace OsnovaLab
{
	namespace IO
	{
		class ScreenIO : public AbstractDeviceIO
		{
		public:
			ScreenIO(Thread::IMutex* mutex);
			virtual ~ScreenIO();

		protected:

            virtual const data_size_t ReadIO(p_data_t data, data_size_t size);
            virtual const data_size_t WriteIO(const p_data_t data, data_size_t size);
			//virtual void ReleaseMutex(Thread::IMutex* mutex);


		private:
			inline void MutexLock();
			inline void MutexUnlock();
		};
	}
}


#endif
