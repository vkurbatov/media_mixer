#ifndef _OL_DATA_MT_QUEUE_H
#define _OL_DATA_MT_QUEUE_H

#include "OsnovaLab\Data\Queue.h"
#include "OsnovaLab\Thread\StdMutex.h"
#include "OsnovaLab\Thread\UniqueLock.h"

namespace OsnovaLab
{
	namespace Data
	{

		template<class T>
		class MtQueue : public Queue<T>
		{
			Thread::StdMutex	mutex_;
		public:
			MtQueue() : Queue<T>()
			{

			}

			virtual ~MtQueue()
			{
			
			}

			//IQueue
			virtual collection_result_t Push(const value_type& data)
			{
				Thread::UniqueLock<IMutex> lock(mutex_);
				Queue<T>::Push(data);
			}

			virtual collection_result_t Pop(value_type* data)
			{
				Thread::UniqueLock<IMutex> lock(mutex_);
				return Queue<T>::Pop(data);
			}

			//ICollection
			virtual void Clear()
			{
				Thread::UniqueLock<IMutex> lock(mutex_);
				return Queue<T>::Clear();
			}
			virtual void Count()
			{
				Thread::UniqueLock<IMutex> lock(mutex_);
				return Queue<T>::Count();
			}

		};
	}
}


#endif

