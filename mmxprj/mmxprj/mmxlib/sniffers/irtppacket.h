#ifndef _MMX_SNIFFERS_I_RTP_PACKET_H
#define _MMX_SNIFFERS_I_RTP_PACKET_H

#include "headers/rtp.h"

namespace mmx
{
    namespace sniffers
    {
        class IRTPPacket
        {
        public:
            virtual ~IRTPPacket(){};
            virtual const headers::RTP_HEADER* Header() const= 0;
            virtual const headers::RTP_EXTENSION_HEADER* ExtensionHeader() const= 0;
            virtual const void* ExtensionData() const = 0;
            virtual const void* Pyload() const = 0;
            virtual int Size() const = 0;
            virtual int PyloadSize() const = 0;

        };
    }
}

#endif
