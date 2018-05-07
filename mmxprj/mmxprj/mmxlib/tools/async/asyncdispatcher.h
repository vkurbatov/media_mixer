#ifndef _MMX_TOOLS_ASYNC_ASYNC_DISPATCHER_H
#define _MMX_TOOLS_ASYNC_ASYNC_DISPATCHER_H

#include "net/iepoll_dispatcher.h"
#include "net/epoll.h"

namespace mmx
{
    namespace tools
    {
        namespace async
        {
            class AsyncDispatcher : net::IEPollDispatcher
            {
                net::EPoll epoll_;

                std::vector<net::IEPollDispatcher*> clients_;

            public:
                AsyncDispatcher();
                ~AsyncDispatcher();

                // IEPollDispatcher interface
                int Dispatch(int fd, int &event_mask) override;
            };
        }
    }
}

#endif
