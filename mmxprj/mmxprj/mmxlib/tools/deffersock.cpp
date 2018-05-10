#include "deffersock.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("DeferredSocket", msg)

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

        bool DeferredSocket::operator== (const DeferredSocket& defsock) const
        {

            return this == &defsock;

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
