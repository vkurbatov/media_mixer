#ifndef _OL_THREAD_STD_THREAD_H
#define _OL_THREAD_STD_THREAD_H

#include <thread>

#include "OsnovaLab/Functional/StdCaller.h"
#include "OsnovaLab/Thread/Thread.h"

namespace OsnovaLab
{
	namespace Thread
	{
		class StdThread : public IThread
		{
			static thread_id_type		g_threadId_;

			thread_id_type				threadId_;
			
			std::thread					thread_;

		public:
			StdThread(Functional::ICaller<>& caller);
			StdThread(Functional::StdCaller<>& caller);
			StdThread(std::function<void()>& fn);
			StdThread(std::function<void()>&& fn);
			virtual ~StdThread();
			virtual void Join();
			virtual thread_id_type GetThreadId() const;
		};
	}
}

#endif
