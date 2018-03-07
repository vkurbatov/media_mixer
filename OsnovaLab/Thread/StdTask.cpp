#include "OsnovaLab/Thread/StdTask.h"
#include "OsnovaLab/Thread/UniqueLock.h"
#include "OsnovaLab/Functional/StdCaller.h"

namespace OsnovaLab
{
	namespace Thread
	{
		int StdTask::g_task_id_ = 0;

		StdTask::StdTask() : 
			task_id_(++g_task_id_),
			thread_(Functional::StdCaller<>([this]{ taskMain(); })),
			run_(true),
			busy_(false)
		{

		}

		StdTask::~StdTask()
		{
			UniqueLock lock(q_mutex_);

			run_ = false;

			cv_.Notify();

			thread_.Join();

		}

        thread_size_t StdTask::PushTask(Functional::ICaller<>* task)
		{
			if (task != nullptr)
			{
				UniqueLock lock(q_mutex_);

				tasks_.push(task);
				
				//if (tasks_.size() > 1)
				//	d = 3;

				cv_.Notify();

				return tasks_.size();
			}
			return -1;
		}

		Functional::ICaller<>* StdTask::PopTask()
		{
			
			Functional::ICaller<>* rc = nullptr;

			UniqueLock lock(q_mutex_);

			if (tasks_.size() > 0)
			{
				rc = tasks_.front();
				tasks_.pop();
			}

			return rc;
		}

        thread_size_t StdTask::GetTaskCount()
		{

			UniqueLock lock(q_mutex_);

			return  tasks_.size();
		}

		bool StdTask::IsEmpty()
		{
			UniqueLock lock(q_mutex_);

			return tasks_.empty();
		}
		highload_factor_type StdTask::GetHighloadFactor()
		{
			UniqueLock lock(q_mutex_);

			// TODO: пока так, но метод должен быть эврестический 

			return (highload_factor_type)tasks_.size() + (highload_factor_type)busy_;
		}

		void StdTask::taskMain()
		{

			//int id = task_id_;

			auto caller = Functional::StdCaller<bool>([&]()
			{
				return  !run_ || !IsEmpty();
			});

			while (run_)
			{
				Functional::ICaller<>* task = nullptr;
				cv_.Wait(TIMEOUT_INFINITE, &caller);
				busy_ = true;
				while (run_ && (task = PopTask()) != nullptr)
				{
					(*task)();
				}
				busy_ = false;
			}

			return;
		}
	}
}
