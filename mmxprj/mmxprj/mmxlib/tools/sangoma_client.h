#ifndef _MMX_TOOLS_SANGOMA_CLIENT_H
#define _MMX_TOOLS_SANGOMA_CLIENT_H

#include "sangoma_client.h"

#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"

#include <list>
#include <vector>

namespace mmx
{
    namespace tools
    {
        class SangomaClient : public IChannelDispatcher
        {

            net::Socket             client_;

            std::vector<char>       data_;

        public:

            SangomaClient(const net::Socket& server, int interval = 2000);
            SangomaClient(SangomaServer&& channel);

            ~PipeOutputChannel() override;
            int Dispatch() override;
            int GetLeftTimeWork() const override;
            int Close() override;
            bool IsDown() const override;


        };
    }
}

#endif
