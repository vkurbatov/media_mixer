#include "OsnovaLab/Thread/StdTaskPool.h"
#include "OsnovaLab/Thread/StdTask.h"
#include "OsnovaLab/Thread/UniqueLock.h"

namespace OsnovaLab
{
	namespace Thread
	{
		StdTaskPool::StdTaskPool(task_size_t task_count) :
			task_count_(task_count < TASKS_MIN ? TASKS_MIN : task_count > TASKS_MAX ? TASKS_MAX : task_count),
			tasks_(task_count_)
		{
			UniqueLock	lock(mutex_);
			for (task_size_t i = 0; i < task_count_; i++)
			{
				//TODO: засунуть через умные указатели!!!
				tasks_[i] = new StdTask();
			}

		}

		StdTaskPool::~StdTaskPool() 
		{
			UniqueLock	lock(mutex_);
			try
			{
				for (auto& t : tasks_)
					delete t;
			}
			catch (std::exception& e)
			{
				//TODO: обработка исключения
			}
		}

		ITask* StdTaskPool::GetTask()
		{
			int i = 0;
			UniqueLock	lock(mutex_);
			ITask* task = nullptr;
			for (auto& t : tasks_)
			{
				if (t->GetHighloadFactor() == TASK_FREE)
				{
					return task = t;
				}
				else
				{

					if (task == nullptr || task->GetHighloadFactor()  > t->GetHighloadFactor())
						task = t;
				}
				i++;
			}
			return task;
		}
	}
}
