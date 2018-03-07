#ifndef _OL_DATA_I_ENUMERABLE_H
#define _OL_DATA_I_ENUMERABLE_H

#include "OsnovaLab/Data/IEnumerator.h"

namespace OsnovaLab
{
	namespace Data
	{
		template<class T>
		class IEnumerable
		{
		public:
            virtual ~IEnumerable() {};
			virtual IEnumerator* GetEnumerator() = 0;
			virtual T& operator[](collection_size_t index) = 0;
		};
	}
}

#endif
