#ifndef _MMX_TOOLS_ORM_SERVER_H
#define _MMX_TOOLS_ORM_SERVER_H

#include "orm_client.h"

#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "tools/timer.h"

#include <list>

namespace mmx
{
    namespace tools
    {
        class OrmServer : public IChannelDispatcher
        {

            std::list<OrmClient>        clients_;

            net::SelectExtension&       select_;
            net::Socket                 socket_;

            tools::Timer                timer_;

            int                         interval_;

            net::address_t              address_;
            net::port_t                 port_;

        public:

            OrmServer(net::address_t address, net::port_t port, net::SelectExtension& select, int interval = 2000);
            OrmServer(OrmServer&& channel);

            ~OrmServer() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int QueryOrderTimeout() const override;
            int Close() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            std::list<OrmClient>& GetClients();

        private:

            int checkConnect();
            int checkClients();

        };
    }
}

#endif
