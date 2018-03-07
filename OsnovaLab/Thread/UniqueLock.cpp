
#include "OsnovaLab/Thread/UniqueLock.h"

namespace OsnovaLab
{
	namespace Thread
	{
		UniqueLock::UniqueLock(IMutex& mutex) : mutex_(&mutex), lock_(0)
		{
			Lock();
		}

		UniqueLock::UniqueLock(IMutex* mutex) : mutex_(mutex), lock_(0)
		{
			Lock();
		}

		UniqueLock::~UniqueLock()
		{
			while (Unlock() > 0);
		}

		int UniqueLock::Lock()
		{
			return mutex_ != nullptr ? lock_ = mutex_->Lock() : -1;
		}

		bool UniqueLock::TryLock()
		{
			bool rc = mutex_ != nullptr ? lock_ = mutex_->TryLock() : false;
			
			lock_ += (int)rc;

			return rc;
		}

		int UniqueLock::Unlock()
		{
			return mutex_ != nullptr ? lock_ = mutex_->Unlock() : -1;
		}
	}
}


