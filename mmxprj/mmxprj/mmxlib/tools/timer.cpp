#include "timer.h"

#include <thread>
#include <climits>

namespace mmx
{
    namespace tools
    {


        Timer::Timer() :
            tout_(-1),
            run_(false),
            start_(std::chrono::system_clock::now())
        {

        }

        void Timer::Start(timer_interval_t interval)
        {

                tout_ = interval;
                start_ = std::chrono::system_clock::now();
                run_ = true;

        }

        bool Timer::Stop()
        {
            run_ = false;
        }

        timer_interval_t Timer::Left() const
        {
            timer_interval_t rc  = 0;

            if (IsStarted())
            {
                if (tout_ < 0)
                {
                    rc = LONG_MAX - Elapsed();
                }
                else
                {
                    if ((rc = tout_ - Elapsed()) < 0)
                    {
                        rc = 0;
                    }
                }
            }

            return rc;
        }

        timer_interval_t Timer::Elapsed() const
        {
            timer_interval_t rc  = 0;

            if (IsStarted())
            {
                rc = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count();
            }

            return rc;
        }

        bool Timer::IsEnable() const
        {
            return run_ && Left() <= 0;
        }

        bool Timer::IsStarted() const
        {
            return run_;
        }

        void Timer::Sleep(timer_interval_t interval)
        {

            std::this_thread::sleep_for(std::chrono::milliseconds(interval));

        }
    }
}
