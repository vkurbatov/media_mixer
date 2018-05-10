#ifndef _MMX_TOOLS_TIMER_H
#define _MMX_TOOLS_TIMER_H

//#include <chrono>

namespace mmx
{
    namespace tools
    {

        using timer_interval_t = int;

        class Timer
        {
            timer_interval_t interval_;
            bool run_;

            unsigned int start_;

            //std::chrono::time_point<std::chrono::system_clock> start_;

        public:
            static unsigned int GetTicks();
            Timer(timer_interval_t interval = -1, bool start = false);
            Timer(Timer&& timer);
            void Start(timer_interval_t interval);
            void Start();
            bool Stop();
            timer_interval_t Elapsed() const;
            timer_interval_t Left() const;
            timer_interval_t Interval() const;
            bool IsEnable() const;
            bool IsStarted() const;

            static void Sleep(timer_interval_t interval);
        };
    }
}

#endif
