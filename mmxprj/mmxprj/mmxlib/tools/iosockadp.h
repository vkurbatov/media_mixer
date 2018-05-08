#ifndef _MMX_TOOLS_IO_SOCK_ADP_H
#define _MMX_TOOLS_IO_SOCK_ADP_H

#include "ipc/io.h"
#include "net/socket.h"

namespace mmx
{
    namespace tools
    {
        class SocketIOAdapter : public ipc::IIO
        {
            net::Socket& socket_;

            net::address_t* address_to_;
            net::port_t* port_to_;
            net::address_t* address_from_;
            net::port_t* port_from_;
        public:
            SocketIOAdapter(net::Socket& socket,
                            net::address_t* address_to = nullptr,
                            net::port_t* port_to = nullptr,
                            net::address_t* address_from = nullptr,
                            net::port_t* port_from = nullptr);

            // IIO
            int Write(const void* data, int size, int flags = 0) override;
            int Read(void* data, int size, int flags = 0) override;
            bool IsCanWrite() const override;
            bool IsCanRead() const override;
        };
    }
}

#endif
