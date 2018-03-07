#ifndef _OL_THREAD_STD_MUTEX_H
#define _OL_THREAD_STD_MUTEX_H

//stl
#include <mutex>

//osnovalab
#include "OsnovaLab/Thread/Mutex.h"

namespace OsnovaLab
{
	namespace Thread
	{
		class StdMutex : public IMutex
		{
			using mutex_type = std::recursive_mutex;

			int							lock_count_;
			mutex_type					mutex_;
			StdMutex*					link_;
		public:
			//IMutex

            virtual ~StdMutex() override;
            virtual int Lock() override;
            virtual bool TryLock() override;
            virtual int Unlock() override;

			//StdMutex
			StdMutex();
			explicit StdMutex(StdMutex& simpleMutext);
			const StdMutex& operator= (StdMutex& op);
			mutex_type* GetNativeMutex();

		};
	}
}
#endif
