#ifndef _OL_STD_SLOT_H
#define _OL_STD_SLOT_H

#include <queue>

#include "OsnovaLab/Thread/StdMutex.h"
#include "OsnovaLab/Thread/Slot.h"
#include "OsnovaLab/Thread/StdSignal.h"

namespace OsnovaLab
{
	namespace Thread
	{
		class StdSlot: public ISlot
		{
			std::queue<signal_type>				signals_;
			IConditionVariable&					cv_;
			StdMutex							q_mutex_;

			StdSlot(IConditionVariable& cv);

		public:
			friend StdSignal;

			virtual ~StdSlot();
			virtual signal_type GetSignal(signal_type signal, timeout_type timeout = 0);
		private:
			void PutSignal(const signal_type signal);
			bool PopSignal(signal_type& signal);
		};
	}
}

#endif
