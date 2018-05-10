#include "timer.h"

#include <climits>

#include <unistd.h>
#include <time.h>

namespace mmx
{
    namespace tools
    {


        Timer::Timer(timer_interval_t interval, bool start) :
            interval_(interval),
            run_(false),
            start_(GetTicks())
        {
            if (start)
            {
                Start();
            }
        }

        Timer::Timer(Timer&& timer) :
            interval_(timer.interval_),
            run_(timer.run_),
            start_(timer.start_)
        {

            timer.interval_ = -1;
            timer.run_ = false;

        }

        unsigned int Timer::GetTicks()
        {

            struct ::timespec ts;

            clock_gettime(CLOCK_REALTIME, &ts );

            return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
        }

        void Timer::Start(timer_interval_t interval)
        {

            interval_ = interval;
            start_ = GetTicks();
            run_ = true;

        }

        void Timer::Start()
        {
            Start(interval_);
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
                if (interval_ < 0)
                {
                    rc = LONG_MAX - Elapsed();
                }
                else
                {
                    if ((rc = interval_ - Elapsed()) < 0)
                    {
                        rc = 0;
                    }
                }
            }

            return rc;
        }

        timer_interval_t Timer::Interval() const
        {
            return interval_;
        }

        timer_interval_t Timer::Elapsed() const
        {
            timer_interval_t rc  = 0;

            if (IsStarted())
            {
                rc = GetTicks() - start_;
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

            ::usleep(interval * 1000);

        }
    }
}
