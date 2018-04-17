#ifndef _MMX_TOOLS_SANGOMA_CLIENT_H
#define _MMX_TOOLS_SANGOMA_CLIENT_H

#include "sangoma_client.h"

#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "tools/iosockadp.h"
#include "tools/deffwriter.h"
#include "headers/si.h"

#include <list>
#include <vector>

namespace mmx
{
    namespace tools
    {
        class SangomaServer;

        class SangomaClient : public IChannelDispatcher
        {

            net::Socket                     socket_;
            SocketIOAdapter                 adapter_;
            DeferredWriter                  writer_;

            net::SelectExtension&           select_;

            std::vector<char>               data_;
            const headers::SANGOMA_PACKET*  query_;

            friend class SangomaServer;
            SangomaClient(net::Socket&& socket, net::SelectExtension& select);

        public:

            SangomaClient(SangomaClient&& channel);

            ~SangomaClient() override;
            int Dispatch() override;
            int GetLeftTimeWork() const override;
            int Close() override;
            bool IsDown() const override;

            const headers::SANGOMA_PACKET* GetQuery();
            int PutAnswer(const headers::SANGOMA_PACKET& answer);
            void Drop();

        private:

            int putData(const void* data = nullptr, int size = 0);

        };
    }
}

#endif
