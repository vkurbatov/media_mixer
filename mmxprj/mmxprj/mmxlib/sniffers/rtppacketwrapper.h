#ifndef _MMX_SNIFFERS_I_RTP_PACKET_WRAPPER_H
#define _MMX_SNIFFERS_I_RTP_PACKET_WRAPPER_H

#include "irtppacket.h"

namespace mmx
{
    namespace sniffers
    {
        class RTPPacketWrapper
        {
            int     size_;

            const headers::RTP_HEADER*              header_;
            const headers::RTP_EXTENSION_HEADER*    ext_header_;
            const void*                             ext_data_;
            const void*                             pyload_;
            int                                     pyload_size_;

        public:

            RTPPacketWrapper(const void* data, int size);
            bool Load(const void* data, int size);

            const headers::RTP_HEADER* Header() const;
            const headers::RTP_EXTENSION_HEADER* ExtensionHeader() const;
            const void* ExtensionData() const;
            const void* Pyload() const;
            int PyloadSize() const;
            int Size() const;

        };
    }
}

#endif
