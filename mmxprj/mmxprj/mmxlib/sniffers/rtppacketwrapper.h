#ifndef _MMX_SNIFFERS_I_RTP_PACKET_WRAPPER_H
#define _MMX_SNIFFERS_I_RTP_PACKET_WRAPPER_H

#include "irtppacket.h"

namespace mmx
{
    namespace sniffers
    {
        class RTPPacketWrapper
        {
            void*   data_;
            int     size_;

        public:

            RTPPacketWrapper(void* data, int size);
            bool Load(void* data, int size);

            const headers::RTP_HEADER* Header() const;
            const headers::RTP_EXTENSION_HEADER* ExtensionHeader() const;
            const void* ExtensionData() const;
            const void* Pyload() const;
            int Size() const;

        private:

            bool check();

        };
    }
}

#endif
