#include "select.h"

#include <cstring> // memset
#include "errno.h"

namespace mmx
{
    namespace net
    {
        Select::Select() :
            fd_max_(-1)
        {
            std::memset((void*)&sets_, 0, sizeof(sets_));
            std::memset((void*)&result_, 0, sizeof(result_));
            std::memset((void*)&sock_table_, 0, sizeof(sock_table_));
        }

        Select::~Select()
        {

        }

        // главный метод

        int Select::Wait(timeout_t timeout)
        {
            timeval tv = { timeout/ 1000, 1000 * (timeout % 1000) };

            std::memcpy(result_, sets_, sizeof(sets_));

            int rc = select(
                        fd_max_ + 1,
                        &result_[0], //readmask
                        &result_[1], //writemask
                        &result_[2], //exception mask
                        timeout == INFINITE_TIMEOUT ? nullptr : &tv
                    );

            if (rc > 0)
            {

                int signaled_sock = rc;

                for (int i = 0; i < fd_max_ + 1 && signaled_sock > 0; i++)
                {
                    event_mask_t mask_old = set2mask(i, result_, 3);
                    event_mask_t mask_new = mask_old;

                    if (mask_old != S_EV_NONE)
                    {

                        if (sock_table_[i].callback != nullptr)
                        {
                            sock_table_[i].callback(i, mask_new, sock_table_[i].context);

                            if (mask_new != mask_old)
                            {
                                Set(i, mask_new, sock_table_[i].callback, sock_table_[i].context);
                            }
                        }

                        signaled_sock--;
                    }

                }
            }
            else
            {

                std::memset(result_, 0, sizeof(result_));

                rc = rc == 0 ? -ETIMEDOUT : -errno;

            }

            return rc;
        }

        int Select::Set(int fd, event_mask_t mask, ev_callback callback, void* context)
        {
            int rc = -EBADF;

            if (fd >= 0 && fd < FD_SETSIZE)
            {

                if ((mask & S_EV_EIO_MASK) != S_EV_NONE)
                {
                    // добавление маскок в наблюдение

                    rc = Get(fd);

                    for (int i = 0; i < 3; i++)
                    {
                        if ((mask & (1 << i) ) != S_EV_NONE)
                        {
                            FD_SET(fd, &sets_[i]);
                        }
                    }

                    sock_table_[fd].callback = callback;
                    sock_table_[fd].context = context;


                    if (fd_max_ < fd)
                    {
                        fd_max_ = fd;
                    }

                }
                else
                {
                    // удалить из наблюдения

                    FD_CLR(fd, &sets_[0]);
                    FD_CLR(fd, &sets_[1]);
                    FD_CLR(fd, &sets_[2]);

                    sock_table_[fd].callback = nullptr;
                    sock_table_[fd].context = nullptr;


                    if (fd == fd_max_)
                    {
                        // ищем максимальный дескриптор

                        while (fd_max_-- >= 0 && set2mask(fd_max_, sets_, 3) == S_EV_NONE);
                    }
                }
            }

            return rc;
        }

        event_mask_t Select::set2mask(int fd, const fd_set* fds, int fd_cnt)
        {
            event_mask_t rc = S_EV_NONE;

            if (fd >= 0 && fd < FD_SETSIZE)
            {
                for (int i = 0; i < fd_cnt; i++)
                {
                    rc |= (event_mask_t)FD_ISSET(fd, fds) << i;
                    fds++;
                }
            }

            return rc;
        }

        event_mask_t Select::Get(int fd) const
        {
            return set2mask(fd, sets_, 3);
        }

        event_mask_t Select::Result(int fd) const
        {
           return set2mask(fd, result_, 3);
        }

        void Select::Reset()
        {
            std::memset(sets_, 0, sizeof(sets_));
            std::memset(result_, 0, sizeof(result_));
            std::memset((void*)&sock_table_, 0, sizeof(sock_table_));

            fd_max_ = -1;
        }

        bool Select::IsRead(int fd) const
        {
            return fd >= 0 && fd < FD_SETSIZE && FD_ISSET(fd,&result_[0]);
        }

        bool Select::IsWrite(int fd) const
        {
            return fd >= 0 && fd < FD_SETSIZE && FD_ISSET(fd,&result_[1]);
        }

        bool Select::IsExcept(int fd) const
        {
            return fd >= 0 && fd < FD_SETSIZE && FD_ISSET(fd,&result_[2]);
        }

        bool Select::IsEmpty() const
        {
            return fd_max_ == -1;
        }
    }
}
