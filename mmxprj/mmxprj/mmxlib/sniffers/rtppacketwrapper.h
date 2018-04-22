#ifndef _MMX_SNIFFERS_I_RTP_PACKET_WRAPPER_H
#define _MMX_SNIFFERS_I_RTP_PACKET_WRAPPER_H

#include "irtppacket.h"

namespace mmx
{
    namespace sniffers
    {
        class RTPPacketWrapper : public IRTPPacket
        {
            int     size_;

            const headers::RTP_HEADER*              header_;
            const headers::RTP_EXTENSION_HEADER*    ext_header_;
            const void*                             ext_data_;
            const void*                             pyload_;
            int                                     pyload_size_;

        public:

            RTPPacketWrapper(const void* data = nullptr, int size = 0);
            bool Load(const void* data, int size);

            const headers::RTP_HEADER* Header() const override;
            const headers::RTP_EXTENSION_HEADER* ExtensionHeader() const override;
            const void* ExtensionData() const override;
            const void* Pyload() const override;
            int PyloadSize() const override;
            int Size() const override;

        };
    }
}

#endif
