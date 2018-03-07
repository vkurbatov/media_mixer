#ifndef _OL_DATA_I_COLLECTION_H
#define _OL_DATA_I_COLLECTION_H

#include "OsnovaLab/Data/DataTypes.h"

namespace OsnovaLab
{
	namespace Data
	{
		template<class T>
		class ICollection
		{
		public:
            virtual ~ICollection() {};
			virtual void Clear() = 0;
			virtual const collection_size_t Count() const = 0;
		};
	}
}

#endif
