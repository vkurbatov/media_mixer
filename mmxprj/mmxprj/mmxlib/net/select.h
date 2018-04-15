#ifndef _MMX_NET_SELECT_H
#define _MMX_NET_SELECT_H

#include <sys/socket.h>


namespace mmx
{
    namespace net
    {

        typedef unsigned int event_mask_t;

        typedef int timeout_t;                              //msec

        static const event_mask_t S_EV_NONE =      0;          // 0
        static const event_mask_t S_EV_READ =      1 << 0;     // 1
        static const event_mask_t S_EV_WRITE =     1 << 1;     // 2
        static const event_mask_t S_EV_EXEPTION =  1 << 2;     // 4
        static const event_mask_t S_EV_EIO_MASK = S_EV_READ | S_EV_WRITE | S_EV_EXEPTION;

        static const timeout_t INFINITE_TIMEOUT = -1;

        typedef int(*ev_callback)(int, event_mask_t&, void*);

        class Select
        {
            struct
            {
//              event_mask_t    set_mask;
                void*           context;
                ev_callback     callback;
            } sock_table_[FD_SETSIZE];

            fd_set  sets_[3];
            fd_set  result_[3];          

            int fd_max_;
            int fd_min_;

            static event_mask_t set2mask(int fd, const fd_set* fds, int fd_cnt);

        public:

            Select();
            virtual ~Select();
            int Wait(timeout_t timeout = INFINITE_TIMEOUT);
            int Set(int fd, event_mask_t mask = S_EV_NONE, ev_callback callback = nullptr, void* context = nullptr);
            event_mask_t Get(int fd) const;
            event_mask_t Result(int fd) const;

            /*
            bool IsRead(int fd) const;
            bool IsWrite(int fd) const;
            bool IsExcept(int fd) const;

            bool SetWrite(int fd);
            bool SetRead(int fd);
            bool SetExcept(int fd);

            bool ClrWrite(int fd);
            bool ClrRead(int fd);
            bool ClrExcept(int fd);
            */

            void Reset();
            bool IsEmpty() const;


        };
    }
}

#endif
