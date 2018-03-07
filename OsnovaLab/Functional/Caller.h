#ifndef _OL_CALLER_H
#define _OL_CALLER_H

namespace OsnovaLab
{
	namespace Functional
	{
		template<class T = void>
		class ICaller
		{
		public:
            virtual ~ICaller() { };
			virtual T operator ()() = 0;
			virtual ICaller<T>* Clone() = 0;
		};
	}
}

#endif
