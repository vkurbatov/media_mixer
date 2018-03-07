#include <chrono>

#include "OsnovaLab/Thread/StdConditionVariable.h"


namespace OsnovaLab
{
	namespace Thread
	{
		StdConditionVariable::StdConditionVariable()
		{
			//std::unique_lock<std::mutex> lock(mutex_);
		}

		StdConditionVariable::~StdConditionVariable()
		{

		}


		int StdConditionVariable::Wait(timeout_type timeout, Functional::ICaller<bool>* predicate)
		{
			int rc = 1;
			std::unique_lock<std::mutex> lock(mutex_);
			

			auto lambda = [predicate]() { return (*predicate)(); };

			if (timeout == TIMEOUT_INFINITE)
			{
				if (predicate == nullptr)
					cv_.wait(lock);
				else
					cv_.wait(lock, lambda);
			}
			else
			{
				if (predicate == nullptr)
				{
					if (cv_.wait_for(lock, std::chrono::milliseconds(timeout)) == std::cv_status::timeout)
						rc = 0;
				}
				else
				{
					if (!cv_.wait_for(lock, std::chrono::milliseconds(timeout), lambda))
						rc = 0;
				}
			}
			return rc;
		}

		int StdConditionVariable::Notify(bool all)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			if (all)
			{
				cv_.notify_all();
			}
			else
			{
				cv_.notify_one();
			}
			return 1;
		}
	}
}
