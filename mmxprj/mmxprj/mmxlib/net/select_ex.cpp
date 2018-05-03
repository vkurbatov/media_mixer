#include "select_ex.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("SelectExtension", msg)

namespace mmx
{
    namespace net
    {
        SelectExtension::SelectExtension() :Select()
        {
            DLOGT(LOG_BEGIN("SelectExtension()"));
        }

        SelectExtension::~SelectExtension()
        {
            DLOGT(LOG_BEGIN("~SelectExtension()"));
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
            DLOGT(LOG_BEGIN("SetWrite(%d)"), fd);
            event_mask_t mask = Get(fd);
            Set(fd, mask | S_EV_WRITE);
            return (mask & S_EV_WRITE) != S_EV_NONE;

        }

        bool SelectExtension::SetRead(int fd)
        {
            DLOGT(LOG_BEGIN("SetRead(%d)"), fd);
            event_mask_t mask = Get(fd);
            Set(fd, mask | S_EV_READ);
            return (mask & S_EV_READ) != S_EV_NONE;
        }

        bool SelectExtension::SetExcept(int fd)
        {
            DLOGT(LOG_BEGIN("SetExcept(%d)"), fd);
            event_mask_t mask = Get(fd);
            Set(fd, mask | S_EV_EXEPTION);
            return (mask & S_EV_EXEPTION) != S_EV_NONE;
        }


        bool SelectExtension::ClrWrite(int fd)
        {
            DLOGT(LOG_BEGIN("ClrWrite(%d)"), fd);
            event_mask_t mask = Get(fd);
            Set(fd, mask & ~S_EV_WRITE);
            return (mask & S_EV_WRITE) != S_EV_NONE;
        }

        bool SelectExtension::ClrRead(int fd)
        {
            DLOGT(LOG_BEGIN("ClrRead(%d)"), fd);
            event_mask_t mask = Get(fd);
            Set(fd, mask & ~S_EV_READ);
            return (mask & S_EV_READ) != S_EV_NONE;
        }

        bool SelectExtension::ClrExcept(int fd)
        {
            DLOGT(LOG_BEGIN("ClrExcept(%d)"), fd);
            event_mask_t mask = Get(fd);
            Set(fd, mask & ~S_EV_EXEPTION);
            return (mask & S_EV_EXEPTION) != S_EV_NONE;
        }

    }
}
