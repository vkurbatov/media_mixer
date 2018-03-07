#ifndef _OL_THREAD_SIGNAL_H
#define _OL_THREAD_SIGNAL_H

#include "OsnovaLab/Thread/ThreadTypes.h"
#include "OsnovaLab/Thread/Slot.h"

namespace OsnovaLab
{
	namespace Thread
	{

		class ISignal
		{
		public:
		

            virtual ~ISignal() {};
			virtual signal_type SendSignal(signal_type singnal = SIGNAL_ANY) = 0;
	
			virtual ISlot* QuerySlot() = 0;
			virtual void ReleaseSlot(ISlot* slot) = 0;
			virtual size_t	GetSlotCount() = 0;
			

			//virtual signal_type Signaling();
		};
	}
}

#endif
