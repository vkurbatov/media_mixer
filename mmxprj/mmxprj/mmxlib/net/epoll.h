#ifndef _MMX_NET_EPOLL_H
#define _MMX_NET_EPOLL_H

#include "iepoll_dispatcher.h"
#include <vector>

namespace mmx
{
    namespace net
    {

        static const int EPOLL_MAX_FD = 1024;

        typedef struct
        {
            int     event_mask;

        }epoll_events_t;

        class EPoll
        {
            int                             handle_;
            int                             count_;

            IEPollDispatcher&               epoll_dispatcher_;

            std::vector<char>               data_;
            std::vector<epoll_events_t>     set_;

        public:
            EPoll(IEPollDispatcher& epoll_dispatcher);
            ~EPoll();

            int Open(int flags = 0);
            int Close();

            int Set(int fd, int event_mask);
            int Clr(int fd, int event_mask);
            int Clear();

            int Wait(int timeout = -1);
            int Count() const;
            int Handle() const;

        private:
            int getIndex(int fd);
            int add(int fd, int event_mask);
            int mod(int fd, int event_mask);
            int del(int fd);
            int dispatch(int fd, int mask);
        };
    }
}

#endif
