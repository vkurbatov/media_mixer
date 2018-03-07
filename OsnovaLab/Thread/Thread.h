#ifndef _OL_THREAD_H
#define _OL_THREAD_H

#include "OsnovaLab/Thread/ThreadTypes.h"

namespace OsnovaLab
{
	namespace Thread
	{
		class IThread
		{
		public:

			virtual ~IThread() {};
			virtual void Join() = 0;
			virtual thread_id_type GetThreadId() const = 0;
		};
	}
}

#endif
