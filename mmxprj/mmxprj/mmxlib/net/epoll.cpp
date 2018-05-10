#include "epoll.h"
#include <cstring>      //memset

#include <errno.h>
#include <unistd.h>     // close
#include <fcntl.h>      // fcntl
#include <sys/epoll.h>  // epoll

namespace mmx
{
    namespace net
    {

        EPoll::EPoll(IEPollDispatcher &epoll_dispatcher) :
            epoll_dispatcher_(epoll_dispatcher),
            handle_(-1),
            count_(0),
            data_(64),
            set_(64)
        {

        }

        EPoll::~EPoll()
        {

        }

        int EPoll::Open(int flags)
        {
            int rc = -EBUSY;

            if (handle_ < 0)
            {
                rc = ::epoll_create1(flags);

                if (rc >= 0)
                {

                }
            }

            return rc;
        }

        int EPoll::Close()
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                rc = close(handle_);

                if (rc < 0)
                {
                    rc = -errno;
                    //data_.clear();
                }

                handle_ = -1;
            }

            return rc;
        }

        int EPoll::Set(int fd, int event_mask)
        {
            int rc = getIndex(fd);

            if (rc >= 0)
            {
                epoll_events_t& ev = set_[rc];

                if (ev.event_mask != event_mask)
                {
                    if (event_mask == 0)
                    {
                        rc = del(fd);
                    }
                    else if (ev.event_mask == 0)
                    {
                        rc = add(fd, event_mask);
                    }
                    else
                    {
                        rc = mod(fd, event_mask);
                    }

                    if (rc >= 0)
                    {
                        rc = ev.event_mask;
                        ev.event_mask = event_mask;
                    }
                }
                else
                {
                    rc = 0;
                }
            }

            return rc;
        }

        int EPoll::Clr(int fd, int event_mask)
        {

            int rc = getIndex(fd);

            if (rc >= 0)
            {
                epoll_events_t& ev = set_[rc];

                if ((ev.event_mask & event_mask) != 0)
                {
                    event_mask = ev.event_mask & ~event_mask;

                    if (event_mask == 0)
                    {
                        rc = del(fd);
                    }
                    else
                    {
                        rc = mod(fd, event_mask);
                    }

                    if (rc >= 0)
                    {
                        rc = ev.event_mask;
                        ev.event_mask = event_mask;
                    }
                }
                else
                {
                    rc = 0;
                }
            }

            return rc;

        }

        int EPoll::Clear()
        {
            int i = 0;
            for (auto& e : set_)
            {
                if (e.event_mask != 0)
                {
                    Set(i, 0);
                }
                i++;
            }
        }

        int EPoll::Wait(int timeout)
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                ::epoll_event* events = (::epoll_event*)data_.data();
                rc = epoll_wait(handle_, events, count_, timeout);

                if (rc == 0)
                {
                    int i = 0;
                    for (auto& e : set_)
                    {
                        if (e.event_mask != 0)
                        {
                            dispatch(i, 0);
                        }
                        i++;
                    }
                    rc = -ETIMEDOUT;
                }
                else if (rc > 0)
                {
                    for (int i = 0; i < rc; i++)
                    {
                        dispatch(events[i].data.fd, events[i].events);
                    }
                }

            }

            return rc;
        }

        int EPoll::Count() const
        {
            return count_;
        }

        int EPoll::Handle() const
        {
            return handle_;
        }

        int EPoll::getIndex(int fd)
        {
            int rc = -EBADF;

            if (fd >= 0 && fd < EPOLL_MAX_FD)
            {
                if (set_.size() < fd)
                {
                    set_.resize(fd + 1);
                }
                rc = fd;
            }

            return rc;
        }

        int EPoll::add(int fd, int event_mask)
        {
            int rc = -EINVAL;

            if (fd >= 0 && event_mask != 0)
            {
                rc = -EBADF;

                if (handle_ >= 0)
                {
                    ::epoll_event ev;
                    ev.data.fd = fd;
                    ev.events = event_mask;

                    rc = ::epoll_ctl(handle_, EPOLL_CTL_ADD, fd, &ev);

                    if (rc >= 0)
                    {
                        rc = ++count_;

                        if (data_.size() < count_ * sizeof(::epoll_event))
                        {
                            data_.resize(data_.size() * 2);
                        }
                    }
                    else
                    {
                        rc = -errno;
                    }
                }
            }

            return rc;
        }

        int EPoll::mod(int fd, int event_mask)
        {
            int rc = -EINVAL;

            if (fd >= 0 && event_mask != 0)
            {
                rc = -EBADF;

                if (handle_ >= 0)
                {
                    ::epoll_event ev;
                    ev.data.fd = fd;
                    ev.events = event_mask;

                    rc = ::epoll_ctl(handle_, EPOLL_CTL_MOD, fd, &ev);

                    if (rc >= 0)
                    {
                        rc = count_;
                    }
                    else
                    {
                        rc = -errno;
                    }
                }
            }

            return rc;
        }

        int EPoll::del(int fd)
        {
            int rc = -EINVAL;

            if (fd >= 0)
            {
                rc = -EBADF;

                if (handle_ >= 0)
                {

                    rc = ::epoll_ctl(handle_, EPOLL_CTL_DEL, fd, nullptr);

                    if (rc >= 0)
                    {
                        rc = --count_;
                    }
                    else
                    {
                        rc = -errno;
                    }
                }
            }

            return rc;
        }

        int EPoll::dispatch(int fd, int mask)
        {

            int o_mask = mask;
            int rc = epoll_dispatcher_.Dispatch(fd, mask);

            if (mask != o_mask)
            {
                if (rc > 0)
                {
                    rc = Set(fd, mask);
                }
                else if (rc < 0)
                {
                    rc = Clr(fd, mask);
                }
            }

            return rc;
        }
    }
}
