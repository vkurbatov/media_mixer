#ifndef _OL_TOOLS_NET_I_DIRECTION_H
#define _OL_TOOLS_NET_I_DIRECTION_H

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/Client.h"

namespace OsnovaLab
{
    namespace Tools
    {
        namespace Net
        {
            class IDirection: public Sockets::IClient
            {

            public:
                virtual ~IDirection() {};
                virtual Sockets::socket_result_t Dispatch(const Sockets::event_mask_t& eventMask) = 0;
                //virtual Sockets::IClient* GetClient() = 0;
            };
        }
    }
}

#endif
