#ifndef _OL_SLOT_H
#define _OL_SLOT_H

#include "OsnovaLab/Thread/ThreadTypes.h"
//#include "OsnovaLab\Thread\Signal.h" 

namespace OsnovaLab
{
	namespace Thread
	{
		class ISlot
		{
			
		public:
            virtual ~ISlot() {};
			virtual signal_type GetSignal(signal_type signal, timeout_type timeout = 0) = 0;
		};
	}
}

#endif
