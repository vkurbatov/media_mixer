#ifndef _OL_DATA_I_ENUMERATOR_H
#define _OL_DATA_I_ENUMERATOR_H

#include "OsnovaLab/Data/DataTypes.h"

namespace OsnovaLab
{
	namespace Data
	{
		template<class T>
		class IEnumerator
		{
		public:
            virtual ~IEnumerator() {};
			virtual T* Current() = 0;
			virtual void MoveNext() = 0;
			virtual void Reset() = 0;
		};
	}
}

#endif
