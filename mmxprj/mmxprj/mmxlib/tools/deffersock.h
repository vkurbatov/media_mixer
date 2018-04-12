#ifndef _MMX_TOOLS_DEFERRED_SOCKET_H
#define _MMX_TOOLS_DEFERRED_SOCKET_H

#include "deffwriter.h"
#include "iosockadp.h"

namespace mmx
{
    namespace tools
    {
        class DeferredSocket
        {
            int limit_;
            net::Socket socket_;
            SocketIOAdapter adapter_;
            DeferredWriter writer_;

        public:
            DeferredSocket(net::Socket&& socket, int limit = 0);
            DeferredSocket(DeferredSocket&& defsock);
            /*DeferredSocket& operator=(DeferredSocket&& defsock);
            DeferredSocket& operator=(net::Socket&& socket);*/
            DeferredWriter& Writter();
            net::Socket& Socket();

        };
    }
}

#endif
