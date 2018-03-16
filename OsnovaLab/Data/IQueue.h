#ifndef _OL_DATA_I_QUEUE_H
#define _OL_DATA_I_QUEUE_H

#include "OsnovaLab/Data/DataTypes.h"

namespace OsnovaLab
{
	namespace Data
	{

		template<class T>
		class IQueue
		{
		public:
			using value_type = T;

            virtual ~IQueue() {};
			virtual collection_result_t Push(const value_type& data) = 0;
            virtual collection_result_t Push(value_type&& data) = 0;
            virtual collection_result_t Pop(value_type* data) = 0;
            virtual collection_result_t Pop(value_type&& data) = 0;
            virtual  value_type* Front() = 0;
		};
	}
}


#endif
