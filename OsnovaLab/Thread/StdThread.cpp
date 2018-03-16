#include "OsnovaLab/Thread/StdThread.h"


namespace OsnovaLab
{
	namespace Thread
	{
		int StdThread::g_threadId_ = 0;

		StdThread::StdThread(Functional::ICaller<>& caller) :
			thread_([&](){ caller(); }),
			threadId_(++g_threadId_)
		{

		}
		
		StdThread::StdThread(Functional::StdCaller<>& caller) :
			thread_(caller.GetFunc()),
			threadId_(++g_threadId_)
		{

		}
		
		StdThread::StdThread(std::function<void()>& fn) : 
			thread_(fn),
			threadId_(++g_threadId_)

		{

		}

		StdThread::StdThread(std::function<void()>&& fn) :
			thread_(std::move(fn)),
			threadId_(++g_threadId_)
		{

		}

		StdThread::~StdThread()
		{
			Join();			
		}

		void StdThread::Join()
		{ 
			if (thread_.joinable())
				thread_.join();
		}

		thread_id_type StdThread::GetThreadId() const
		{
			auto id = thread_.get_id();
            //TODO:
			return *(thread_id_type*)&id;

		}
	}
}
