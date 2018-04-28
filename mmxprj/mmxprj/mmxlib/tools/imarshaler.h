#ifndef _MMX_TOOLS_I_MARSHALER_H
#define _MMX_TOOLS_I_MARSHALER_H

#include "sniffers/iippacket.h"

namespace mmx
{
    namespace tools
    {

        class IMarshaler
        {
        public:
            virtual int PutData(const sniffers::IIPPacket& ippacket) = 0;
        };

    }
}

#endif
