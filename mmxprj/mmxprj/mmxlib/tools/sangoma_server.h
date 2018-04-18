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

            std::list<SangomaClient>    clients_;

            net::SelectExtension&       select_;
            net::Socket                 socket_;

            tools::Timer                timer_;

            int                         interval_;

            net::address_t              address_;
            net::port_t                 port_;

        public:

            SangomaServer(net::address_t address, net::port_t port, net::SelectExtension& select, int interval = 2000);
            SangomaServer(SangomaServer&& channel);

            ~SangomaServer() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int QueryOrderTimeout() const override;
            int Close() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            std::list<SangomaClient>& GetClients();

        private:

            int checkConnect();
            int checkClients();

        };
    }
}

#endif
