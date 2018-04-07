#ifndef _MMX_TOOLS_TIMER_H
#define _MMX_TOOLS_TIMER_H

#include <chrono>

namespace mmx
{
    namespace tools
    {

        using timer_interval_t = int;

        class Timer
        {
            timer_interval_t tout_;
            bool run_;

            std::chrono::time_point<std::chrono::system_clock> start_;

        public:
            Timer();
            void Start(timer_interval_t interval);
            bool Stop();
            timer_interval_t Elapsed() const;
            timer_interval_t Left() const;
            bool IsEnable() const;
            bool IsStarted() const;

            static void Sleep(timer_interval_t interval);
        };
    }
}

#endif
