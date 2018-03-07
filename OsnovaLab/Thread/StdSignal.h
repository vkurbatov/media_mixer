#ifndef _OL_STD_SIGNAL_H
#define _OL_STD_SIGNAL_H

#include <list>

#include "OsnovaLab/Thread/StdConditionVariable.h"
#include "OsnovaLab/Thread/StdMutex.h"
#include "OsnovaLab/Thread/Signal.h"
//#include "OsnovaLab/Thread/Slot.h"

namespace OsnovaLab
{
	namespace Thread
	{
		class StdSignal : public ISignal
		{
			StdMutex				l_mutex;
			std::list<ISlot*>		slots_;
			StdConditionVariable	cv_;

		public:
			StdSignal();
			~StdSignal();
			virtual signal_type SendSignal(signal_type signal = SIGNAL_ANY);
			virtual size_t	GetSlotCount();

			virtual ISlot* QuerySlot();
			virtual void ReleaseSlot(ISlot* slot);

			//virtual signal_type Signaling();
		};
	}
}

#endif
