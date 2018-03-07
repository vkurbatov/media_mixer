#ifndef _OL_THREAD_STD_TASK_H
#define _OL_THREAD_STD_TASK_H

#include <queue>

#include "OsnovaLab/Thread/StdThread.h"
#include "OsnovaLab/Thread/StdMutex.h"
#include "OsnovaLab/Thread/StdConditionVariable.h"
#include "OsnovaLab/Thread/Task.h"
#include "OsnovaLab/Functional/Caller.h"

namespace OsnovaLab
{
	namespace Thread
	{
		class StdTask : public ITask
		{
			static int								g_task_id_;
			int										task_id_ = 0;
			bool									run_;
			bool									busy_;
			std::queue<Functional::ICaller<>*>		tasks_;
			StdMutex								q_mutex_;
			StdConditionVariable					cv_;
			StdThread								thread_;

		public:
			StdTask();
			virtual ~StdTask();
            virtual thread_size_t PushTask(Functional::ICaller<>* task);
			virtual Functional::ICaller<>* PopTask();
            virtual thread_size_t GetTaskCount();
			virtual bool IsEmpty();
			virtual highload_factor_type GetHighloadFactor();

		private:
			void taskMain();
		};
	}
}

#endif
