#include "deffersock.h"

namespace mmx
{
    namespace tools
    {
        DeferredSocket::DeferredSocket(net::Socket&& socket, int limit) :
            limit_(limit),
            socket_(std::move(socket)),
            adapter_(socket_),
            writer_(adapter_, limit_)
        {

        }

        DeferredSocket::DeferredSocket(DeferredSocket&& defsock) :
            limit_(defsock.limit_),
            socket_(std::move(defsock.socket_)),
            adapter_(socket_),
            writer_(adapter_, limit_)
        {

        }
/*
        DeferredSocket& DeferredSocket::operator=(DeferredSocket&& defsock)
        {
            socket_ = std::move(defsock.socket_);
        }

        DeferredSocket& DeferredSocket::operator=(net::Socket&& socket)
        {
            socket_ = std::move(socket);
        }
*/

        DeferredWriter& DeferredSocket::Writter()
        {

            return writer_;

        }

        net::Socket& DeferredSocket::Socket()
        {

            return socket_;

        }
    }
}
