#ifndef _OL_THREAD_CONDITION_VARIABLE_H
#define _OL_THREAD_CONDITION_VARIABLE_H

#include "OsnovaLab/Thread/ThreadTypes.h"
#include "OsnovaLab/Functional/Caller.h"

namespace OsnovaLab
{
	namespace Thread
	{

		class IConditionVariable
		{
		public:
            virtual ~IConditionVariable() {};
			virtual int Wait(timeout_type timeout = TIMEOUT_NULL, Functional::ICaller<bool>* predicate = nullptr) = 0;
			virtual int Notify(bool all = false) = 0;
		};
	}
}
#endif
