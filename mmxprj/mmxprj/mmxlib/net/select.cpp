#include "select.h"

#include <cstring> // memset
#include "errno.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("Select", msg)

namespace mmx
{
    namespace net
    {
        Select::Select() :
            fd_max_(-1),
            fd_min_(-1)
        {

            DLOGT(LOG_BEGIN("Select()"));

            std::memset((void*)&sets_, 0, sizeof(sets_));
            std::memset((void*)&result_, 0, sizeof(result_));
            std::memset((void*)&sock_table_, 0, sizeof(sock_table_));
        }

        Select::~Select()
        {
            DLOGT(LOG_BEGIN("~Select()"));
        }

        // главный метод

        int Select::Wait(timeout_t timeout)
        {
            timeval tv = { timeout/ 1000, 1000 * (timeout % 1000) };

            DLOGT(LOG_BEGIN("Wait(%d)"), timeout);

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

                DLOGT(LOG_BEGIN("Wait(%d): select[%d..%d] return %d"), timeout, fd_min_, fd_max_ + 1, rc);

                int signaled_sock = rc;

                for (int i = fd_min_; i < fd_max_ + 1 && signaled_sock > 0; i++)
                {
                    event_mask_t mask_old = set2mask(i, result_, 3);
                    event_mask_t mask_new = mask_old;

                    if (mask_old != S_EV_NONE)
                    {
                        DLOGT(LOG_BEGIN("Wait(): fd %d mask = %x"), i, mask_old);

                        if (sock_table_[i].callback != nullptr)
                        {
                            sock_table_[i].callback(i, mask_new, sock_table_[i].context);

                            if (mask_new != mask_old)
                            {
                                Set(i, mask_new, sock_table_[i].callback, sock_table_[i].context);
                            }
                        }

                        const static int num_bit_tbl[] = { 0, 1, 1, 2, 1, 2, 2, 3};

                        signaled_sock -= num_bit_tbl[ mask_old & 0x07];

                    }

                }
            }
            else
            {

                std::memset(result_, 0, sizeof(result_));

                rc = rc == 0 ? -ETIMEDOUT : -errno;

                if (rc == -ETIMEDOUT)
                {
                    DLOGT(LOG_BEGIN("Wait(%d): timeout"), timeout);
                }
                else
                {
                    DLOGE(LOG_BEGIN("Wait(%d): select error = %d"), rc);
                }

            }

            return rc;
        }

        int Select::Set(int fd, event_mask_t mask, ev_callback callback, void* context)
        {
            int rc = -EBADF;

            DLOGT(LOG_BEGIN("Set(%d, %x, %x, %x)"), fd, mask, DLOG_POINTER(callback), DLOG_POINTER(context));

            if (fd >= 0 && fd < FD_SETSIZE)
            {

                if ((mask & S_EV_EIO_MASK) != S_EV_NONE)
                {
                    // добавление масок в наблюдение

                    rc = Get(fd);

                    for (int i = 0; i < 3; i++)
                    {
                        if ((mask & (1 << i) ) != S_EV_NONE)
                        {
                            FD_SET(fd, &sets_[i]);
                        }
                        else
                        {
                            FD_CLR(fd, &sets_[i]);
                            FD_CLR(fd, &result_[i]);
                        }
                    }

                    sock_table_[fd].callback = callback;
                    sock_table_[fd].context = context;

                    if (fd_max_ < fd)
                    {
                        fd_max_ = fd;
                    }

                    if (fd_min_ > fd || fd_min_ < 0)
                    {
                        fd_min_ = fd;
                    }

                }
                else
                {
                    // удалить из наблюдения

                    FD_CLR(fd, &sets_[0]);
                    FD_CLR(fd, &sets_[1]);
                    FD_CLR(fd, &sets_[2]);

                    // результат тоже стираем

                    FD_CLR(fd, &result_[0]);
                    FD_CLR(fd, &result_[1]);
                    FD_CLR(fd, &result_[2]);

                    sock_table_[fd].callback = nullptr;
                    sock_table_[fd].context = nullptr;

                    if (fd == fd_max_)
                    {
                        // ищем максимальный дескриптор

                        while (fd_max_-- >= 0 && set2mask(fd_max_, sets_, 3) == S_EV_NONE);
                    }
                    if (fd == fd_min_)
                    {
                        // ищем минимальный дескриптор

                        while (fd_min_ < fd_max_ && set2mask(fd_min_, sets_, 3) == S_EV_NONE)
                        {
                            fd_min_++;
                        }

                        if (fd_min_ > fd_max_)
                        {
                            fd_min_ = fd_max_;
                        }
                    }
                }

                DLOGT(LOG_BEGIN("Set(): set[%d..%d]"), fd_min_, fd_max_);
            }
            else
            {
                 DLOGT(LOG_BEGIN("Set(): invalid fd value = %d"), fd);
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

            DLOGT(LOG_BEGIN("Reset()"));

            std::memset(sets_, 0, sizeof(sets_));
            std::memset(result_, 0, sizeof(result_));
            std::memset((void*)&sock_table_, 0, sizeof(sock_table_));

            fd_max_ = -1;
        }



        bool Select::IsEmpty() const
        {
            return fd_max_ == -1;
        }
    }
}
