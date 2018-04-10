#ifndef _MMX_TOOLS_SOCKDISP_H
#define _MMX_TOOLS_SOCKDISP_H

#include <queue>

#include "net/socket.h"
#include "ipc/io.h"
#include "ipc/pchannel.h"
#include "staff/packet.h"

namespace mmx
{
    namespace tools
    {
        class SockDispatcher : public ipc::IIO
        {
            net::Socket socket_;
            std::queue<staff::Packet>   q_write_;
            std::queue<staff::Packet>   q_read_;

        public:
            SockDispatcher(net::Socket&& socket);
            ~SockDispatcher();

            // IIO
            virtual int Write(const char*, int, int = 0);
            virtual int Read(char*, int, int = 0);

            bool IsWriteEmpty() const;
            bool IsReadEmpty() const;

            void Reset();
        };
    }
}

#endif
