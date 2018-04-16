#ifndef _MMX_TOOLS_SANGOMA_SERVER_H
#define _MMX_TOOLS_SANGOMA_SERVER_H

#include "sangoma_client.h"

#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "tools/timer.h"

#include <list>

namespace mmx
{
    namespace tools
    {
        class SangomaServer : public IChannelDispatcher
        {



        public:

            SangomaServer(net::address_t address, net::port_t port, int interval = 2000);
            SangomaServer(SangomaServer&& channel);

            ~PipeOutputChannel() override;
            int Dispatch() override;
            int GetLeftTimeWork() const override;
            int Close() override;
            bool IsDown() const override;
        };
    }
}

#endif
