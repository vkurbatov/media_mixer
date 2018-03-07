#include <chrono>
#include <functional>

#include "OsnovaLab/Functional/StdCaller.h"
#include "OsnovaLab/Thread/StdSlot.h"

namespace OsnovaLab
{
	namespace Thread
	{

		StdSlot::StdSlot(IConditionVariable& cv) : cv_(cv)
		{

		}

		StdSlot::~StdSlot()
		{

		}

		signal_type StdSlot::GetSignal(signal_type signal, timeout_type timeout)
		{
			signal_type rc = SIGNAL_E_INVALID;

			bool one = false;

			Functional::StdCaller<bool> caller([&]()
			{
				if (rc == SIGNAL_E_INVALID)
				{
					signal_type sgn;
					while (PopSignal(sgn))
					{
						if (sgn == 10)
							sgn = sgn;
						if (signal == SIGNAL_ANY ||
							sgn == SIGNAL_ANY ||
							signal == sgn)
						{
							rc = sgn;
						}
					}
				}
				return rc != SIGNAL_E_INVALID;
			});

			if (cv_.Wait(timeout, &caller) == 0)
			{
				rc = SIGNAL_E_TIMEOUT;
			}

			return rc;
		}

		/*
		signal_type StdSlot::GetSignal(signal_type signal, timeout_type timeout)
		{
			signal_type rc = SIGNAL_E_INVALID;

			bool one = false;

			if (cv_.Wait(timeout) != 0)
			{
				signal_type sgn;
				while (PopSignal(sgn))
				{
					if (sgn == 10)
						sgn = sgn;
					if (signal == SIGNAL_ANY ||
						sgn == SIGNAL_ANY ||
						signal == sgn)
					{
						rc = sgn;
					}
				}
			}
			else
				rc = SIGNAL_E_TIMEOUT;

			return rc;
		}
		*/
		void StdSlot::PutSignal(const signal_type signal)
		{
			int r = 0;
			q_mutex_.Lock();
			signals_.push(signal);
			q_mutex_.Unlock();

		}

		bool StdSlot::PopSignal(signal_type& signal)
		{
			bool rc = false;

			q_mutex_.Lock();

			if (rc = (signals_.size() > 0))
			{
				signal = signals_.front();
				signals_.pop();
			}

			q_mutex_.Unlock();

			return rc;

		}
	}
}
