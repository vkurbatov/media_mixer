#include "OsnovaLab/Thread/StdMutex.h"

namespace OsnovaLab
{
	namespace Thread
	{
		StdMutex::StdMutex() : lock_count_(0), link_(nullptr)
		{
			
		}

		StdMutex::StdMutex(StdMutex& stdMutex) : lock_count_(0), link_(&stdMutex)
		{

		}

		StdMutex::~StdMutex()
		{

		}

		const StdMutex& StdMutex::operator= (StdMutex& op)
		{
			link_ = &op;
			return op;
		}

		int StdMutex::Lock()
		{
			if (link_ != nullptr)
			{
				return link_->Lock();
			}
			mutex_.lock();
			return ++lock_count_;
		}


		int StdMutex::Unlock()
		{
			if (link_ != nullptr)
			{
				return link_->Unlock();
			}
			mutex_.unlock();
			return --lock_count_;
		}

		bool StdMutex::TryLock()
		{
			bool rc = link_ != nullptr ? 
				link_->TryLock() :
				mutex_.try_lock();

			lock_count_ += rc;
			
			return rc;
		}

	}
}
