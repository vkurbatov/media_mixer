#ifndef _OL_MUTEX_H
#define _OL_MUTEX_H

//stl
#include <memory>

namespace OsnovaLab
{
	namespace Thread
	{
		class IMutex
		{
		public:
            virtual ~IMutex() {};
			virtual int Lock() = 0;
			virtual bool TryLock() = 0;
			virtual int Unlock() = 0;
		};

        //TODO:
		typedef std::shared_ptr<IMutex> IMutexSharedPtr;
	}
}
#endif
