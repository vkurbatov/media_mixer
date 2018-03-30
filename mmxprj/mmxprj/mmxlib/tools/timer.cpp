#include "timer.h"

#include <thread>

namespace mmx
{
    namespace tools
    {


        Timer::Timer() :
            tout_(-1),
            start_(std::chrono::system_clock::now())
        {

        }

        void Timer::Start(timer_interval_t interval)
        {
            if (interval >= 0)
            {

                tout_ = interval;
                start_ = std::chrono::system_clock::now();

            }
            else
            {
                tout_ = -1;
            }
        }

        bool Timer::Stop()
        {
            tout_ = -1;
        }

        timer_interval_t Timer::Elapsed() const
        {
            timer_interval_t rc  = 0;

            if (IsStarted())
            {
                rc = tout_ - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count();
            }

            return rc;
        }

        bool Timer::IsEnable() const
        {
            return IsStarted() && Elapsed() <= 0;
        }

        bool Timer::IsStarted() const
        {
            return tout_ >= 0;
        }

        void Timer::Sleep(timer_interval_t interval)
        {

            std::this_thread::sleep_for(std::chrono::milliseconds(interval));

        }
    }
}
