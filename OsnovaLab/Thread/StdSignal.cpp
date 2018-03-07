#include <chrono>
#include <functional>


#include "OsnovaLab/Functional/StdCaller.h"
#include "OsnovaLab/Thread/StdSignal.h"
#include "OsnovaLab/Thread/StdSlot.h"

namespace OsnovaLab
{
	namespace Thread
	{

		StdSignal::StdSignal() : slots_()
		{

		}

		StdSignal::~StdSignal()
		{
			l_mutex.Lock();

			for (auto& s : slots_)
				delete s;

			slots_.clear();

			l_mutex.Unlock();
		}

		signal_type StdSignal::SendSignal(signal_type signal)
		{
			l_mutex.Lock();

			for (auto& s : slots_)
			{
				dynamic_cast<StdSlot*>(s)->PutSignal(signal);
			}

			cv_.Notify(true);

			l_mutex.Unlock();

			return signal;
		}

		size_t StdSignal::GetSlotCount()
		{
			size_t rc = 0;

			l_mutex.Lock();

			rc = slots_.size();

			l_mutex.Unlock();

			return rc;
		}

		ISlot* StdSignal::QuerySlot()
		{
			ISlot* rc = new StdSlot(cv_);

			l_mutex.Lock();

			slots_.push_back(rc);
						
			l_mutex.Unlock();

			return rc;
		}

		void StdSignal::ReleaseSlot(ISlot* slot)
		{
			if (slot != nullptr)
			{
				l_mutex.Lock();

				slots_.remove(slot);
				delete slot;

				l_mutex.Unlock();
			}
		}
	}
}
