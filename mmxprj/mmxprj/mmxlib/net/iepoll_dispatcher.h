#ifndef _MMX_NET_EPOLL_I_DISPATCHER_H
#define _MMX_NET_EPOLL_I_DISPATCHER_H


namespace mmx
{
    namespace net
    {

        class IEPollDispatcher
        {

        public:
            virtual ~IEPollDispatcher() {};

            virtual int Dispatch(int fd, int& event_mask) = 0;
        };
    }
}

#endif
