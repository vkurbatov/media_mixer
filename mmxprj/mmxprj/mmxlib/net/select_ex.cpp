#include "select_ex.h"

namespace mmx
{
    namespace net
    {
        SelectExtension::SelectExtension() :Select()
        {

        }

        SelectExtension::~SelectExtension()
        {

        }

        bool SelectExtension::IsRead(int fd) const
        {
            return (Result(fd) & S_EV_READ) != S_EV_NONE;
        }

        bool SelectExtension::IsWrite(int fd) const
        {
            return (Result(fd) & S_EV_WRITE) != S_EV_NONE;
        }

        bool SelectExtension::IsExcept(int fd) const
        {
            return (Result(fd) & S_EV_EXEPTION) != S_EV_NONE;
        }

        bool SelectExtension::SetWrite(int fd)
        {
            event_mask_t mask = Get(fd);
            Set(fd, mask | S_EV_WRITE);
            return (mask & S_EV_WRITE) != S_EV_NONE;

        }

        bool SelectExtension::SetRead(int fd)
        {
            event_mask_t mask = Get(fd);
            Set(fd, mask | S_EV_READ);
            return (mask & S_EV_READ) != S_EV_NONE;
        }

        bool SelectExtension::SetExcept(int fd)
        {
            event_mask_t mask = Get(fd);
            Set(fd, mask | S_EV_EXEPTION);
            return (mask & S_EV_EXEPTION) != S_EV_NONE;
        }


        bool SelectExtension::ClrWrite(int fd)
        {
            event_mask_t mask = Get(fd);
            Set(fd, mask & ~S_EV_WRITE);
            return (mask & S_EV_WRITE) != S_EV_NONE;
        }

        bool SelectExtension::ClrRead(int fd)
        {
            event_mask_t mask = Get(fd);
            Set(fd, mask & ~S_EV_READ);
            return (mask & S_EV_READ) != S_EV_NONE;
        }

        bool SelectExtension::ClrExcept(int fd)
        {
            event_mask_t mask = Get(fd);
            Set(fd, mask & ~S_EV_EXEPTION);
            return (mask & S_EV_EXEPTION) != S_EV_NONE;
        }

    }
}
