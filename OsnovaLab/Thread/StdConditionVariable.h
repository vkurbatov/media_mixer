#ifndef _OL_STD_CONDITION_VARIABLE_H
#define _OL_STD_CONDITION_VARIABLE_H

#include <condition_variable>
//#include <functional>

#include "OsnovaLab/Thread/ThreadTypes.h"
#include "OsnovaLab/Thread/ConditionVariable.h"

namespace OsnovaLab
{
	namespace Thread
	{

		class StdConditionVariable : public IConditionVariable
		{
			std::condition_variable		cv_;
			std::mutex					mutex_;

		public:
			//this
			StdConditionVariable();

			//IConditionVariable
			virtual ~StdConditionVariable();
			virtual int Wait(timeout_type timeout = TIMEOUT_NULL, Functional::ICaller<bool>* predicate = nullptr);
			virtual int Notify(bool all = false);
		};
	}
}
#endif
