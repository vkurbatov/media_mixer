#ifndef _MMX_SNIFFERS_I_IP_PACKET_H
#define _MMX_SNIFFERS_I_IP_PACKET_H

#include "headers/ip.h"

namespace mmx
{
    namespace sniffers
    {
        class IIPPacket
        {
        public:
            virtual ~IIPPacket(){};
            virtual const headers::IP4HEADER* Header() const= 0;
            virtual const void* Pyload() const = 0;
            virtual int Size() const = 0;

        };
    }
}

#endif
