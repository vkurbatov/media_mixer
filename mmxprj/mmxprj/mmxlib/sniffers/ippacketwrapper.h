#ifndef _MMX_SNIFFER_IP_PACKET_WRAPPER_H
#define _MMX_SNIFFER_IP_PACKET_WRAPPER_H

#include "iippacket.h"

namespace mmx
{
    namespace sniffers
    {
        class IPPacketWrapper : public IIPPacket
        {
            const void* pointer_;
        public:
            IPPacketWrapper(void* pointer = nullptr);

            // IIPPacket
            const headers::IP4HEADER* Header() const override;
            const void* Pyload() const override;
            int Size() const override;

            void* Reset(const void* pointer = nullptr);
        };
    }
}

#endif
