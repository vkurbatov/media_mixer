#include "ippacketwrapper.h"

#include <netdb.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("IPPacketWrapper", msg)

namespace mmx
{
    namespace sniffers
    {
        IPPacketWrapper::IPPacketWrapper(void* pointer) :
            pointer_(pointer)
        {
            DLOGT(LOG_BEGIN("IPPacketWrapper(%x)"), DLOG_POINTER(pointer));
        }

        IPPacketWrapper::~IPPacketWrapper()
        {
            DLOGT(LOG_BEGIN("~IPPacketWrapper()"));
        }

        const headers::IP4HEADER* IPPacketWrapper::Header() const
        {
            return (const headers::IP4HEADER*)pointer_;
        }

        const void* IPPacketWrapper::Pyload() const
        {
            return (char*)pointer_ + Header()->hlen * 4;

        }

        int IPPacketWrapper::Size() const
        {
            return ::ntohs(Header()->length) - Header()->hlen * 4;
        }

        void* IPPacketWrapper::Reset(const void* pointer)
        {
            DLOGT(LOG_BEGIN("Reset(%x)"), DLOG_POINTER(pointer));
            pointer_ = pointer;

        }
    }
}
