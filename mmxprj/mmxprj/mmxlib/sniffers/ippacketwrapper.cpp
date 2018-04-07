#include "ippacketwrapper.h"

#include <netdb.h>

namespace mmx
{
    namespace sniffers
    {
        IPPacketWrapper::IPPacketWrapper(void* pointer) :
            pointer_(pointer)
        {

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

        void* IPPacketWrapper::Reset(void* pointer)
        {

            pointer_ = pointer;

        }
    }
}
