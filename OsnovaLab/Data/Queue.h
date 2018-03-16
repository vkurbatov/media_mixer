#ifndef _OL_DATA_QUEUE_H
#define _OL_DATA_QUEUE_H

#include <queue>
#include <algorithm>

#include "OsnovaLab/Data/IQueue.h"
#include "OsnovaLab/Data/ICollection.h"

namespace OsnovaLab
{
	namespace Data
	{

		template<class T>
		class Queue : public IQueue<T>, ICollection<T>
		{
            using value_type = T;
            std::queue<value_type>	queue_;
		public:
			Queue() : queue_()
			{

			}

            virtual ~Queue() override
			{
				Clear();
			}

			//IQueue
            virtual collection_result_t Push(const value_type& data) override
			{
				queue_.push(data);
				return queue_.size();
			}

            virtual collection_result_t Push(value_type&& data) override
            {
                queue_.push(std::move(data));
                return queue_.size();
            }

            virtual collection_result_t Pop(value_type* data) override
			{
				collection_result_t rc = -1;

				if (!queue_.empty())
				{
					if (data != nullptr)
					{
                        *data = queue_.front();
					}
					queue_.pop();
					rc = queue_.size();
				}

				return rc;
			}

            collection_result_t Pop(value_type&& data) override
            {
                collection_result_t rc = -1;

                if (!queue_.empty())
                {
                    data = std::move(queue_.front());
                    queue_.pop();
                    rc = queue_.size();
                }

                return rc;
            }

            value_type* Front() override
            {
                return queue_.size() > 0 ? &queue_.front() : nullptr;
            }

			//ICollection
            virtual void Clear() override
			{
				while (!queue_.empty())
				{
					queue_.pop();
				}
			}
            virtual const collection_size_t Count() const override
			{
				return queue_.size();
			}
		};
	}
}


#endif

