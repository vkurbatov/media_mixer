#include "iosockadp.h"

namespace mmx
{
    namespace tools
    {
        SocketIOAdapter::SocketIOAdapter(net::Socket &socket,
                                         net::address_t *address_to,
                                         net::port_t *port_to,
                                         net::address_t *address_from,
                                         net::port_t *port_from) :
                socket_(socket),
                address_to_(address_to),
                port_to_(port_to),
                address_from_(address_from),
                port_from_(port_from)
        {

        }

        // IIO
        int SocketIOAdapter::Write(const void* data, int size, int flags)
        {
            return socket_.Send(data,
                                size,
                                flags,
                                address_to_ == nullptr ? 0 : *address_to_,
                                port_to_ == nullptr ? 0 : *port_to_);
        }

        int SocketIOAdapter::Read(void* data, int size, int flags)
        {
            return socket_.Recv(data,
                                size,
                                flags,
                                address_from_,
                                port_from_);
        }

        bool SocketIOAdapter::IsCanWrite() const
        {
            return socket_.Handle() >= 0;
        }

        bool SocketIOAdapter::IsCanRead() const
        {
            return socket_.Handle() >= 0;
        }

    }
}

