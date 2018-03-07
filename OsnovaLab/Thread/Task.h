#ifndef _OL_TASK_H
#define _OL_TASK_H

#include "OsnovaLab/Thread/ThreadTypes.h"
#include "OsnovaLab/Functional/Caller.h"

namespace OsnovaLab
{
	namespace Thread
	{
		using highload_factor_type = int; 
		
		static const highload_factor_type TASK_FREE = 0;

		class ITask
		{
		public:


            virtual ~ITask() {};
            virtual thread_size_t PushTask(Functional::ICaller<>* task) = 0;
			virtual Functional::ICaller<>* PopTask() = 0;
            virtual thread_size_t GetTaskCount() = 0;
			virtual bool IsEmpty() = 0;
			virtual highload_factor_type GetHighloadFactor() = 0;
		};
	}
}

#endif
