#ifndef _OL_UNIQUE_LOCK_H
#define _OL_UNIQUE_LOCK_H

#include "OsnovaLab/Thread/Mutex.h"

namespace OsnovaLab
{
	namespace Thread
	{
		class UniqueLock : public IMutex
		{
			IMutex*		mutex_;
			int			lock_;
		public:
			UniqueLock(IMutex& mutex);
			UniqueLock(IMutex* mutex);
			~UniqueLock();

			//IMutex
			virtual int Lock();
			virtual bool TryLock();
			virtual int Unlock();

		};
	}
}


#endif
