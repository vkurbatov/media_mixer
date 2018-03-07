#ifndef _OL_THREAD_TASK_POOL_H
#define _OL_THREAD_TASK_POOL_H

#include "OsnovaLab/Thread/Task.h"

namespace OsnovaLab
{
	namespace Thread
	{
        using task_size_t = thread_size_t;

		class ITaskPool
		{
		public:
            virtual ~ITaskPool() {};
			virtual ITask* GetTask() = 0;
		};
	}
}

#endif
