#ifndef _MMX_IPC_MSG_CHANNEL_H
#define _MMX_IPC_MSG_CHANNEL_H

#include <memory>
#include <vector>

#include "ichannel.h"
#include "headers/msg.h"

namespace mmx
{
    namespace ipc
    {
        const int DEFAULT_BUFFER_CAP = 1600;

        class MsgChannel
        {
            IChannel& channel_;
            std::vector<unsigned char>  buffer_;
        public:
            MsgChannel(IChannel& channel);
            //static int BuildHeader(headers::PMSG_HEADER pmsg, unsigned short msg, unsigned short );
            int SendMsg(const headers::PMSG_HEADER pmsg, const char* data = nullptr, int flags = 0);
            int RecvMsg(headers::PMSG_HEADER pmsg, char* data = nullptr, unsigned short size = 0, int flags = 0);
        };
    }
}

#endif
