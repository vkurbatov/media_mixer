#ifndef _OL_THREAD_STD_TASK_POOL_H
#define _OL_THREAD_STD_TASK_POOL_H

#include "OsnovaLab/Thread/TaskPool.h"
#include "OsnovaLab/Thread/StdMutex.h"

#include <vector>
#include <memory>

namespace OsnovaLab
{
	namespace Thread
	{

		class StdTaskPool: public ITaskPool
		{
			StdMutex				mutex_;
			task_size_t				task_count_;
			std::vector<ITask*>		tasks_;
		public:
			
			static const task_size_t TASKS_DEFAULT = 10;
			static const task_size_t TASKS_MAX = 1000;
			static const task_size_t TASKS_MIN = 1;

			//ITaskPool

			StdTaskPool(task_size_t task_count = TASKS_DEFAULT);
			virtual ~StdTaskPool();
			virtual ITask* GetTask();

			//StdTaskPool
			/*
			template<class Fx, class... Args>
			void runAsync(cons)*/
		};
	}
}

#endif
