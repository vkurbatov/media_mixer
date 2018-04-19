#ifndef _MMX_TOOLS_TEMPLATE_TCP_SERVER_H
#define _MMX_TOOLS_TEMPLATE_TCP_SERVER_H

#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "tools/timer.h"

#include <list>

namespace mmx
{
    namespace tools
    {
        template<class T>
        class TemplateTcpServer : public IChannelDispatcher
        {

            std::list<T>                clients_;

            net::SelectExtension&       select_;
            net::Socket                 socket_;

            tools::Timer                timer_;

            int                         interval_;

            net::address_t              address_;
            net::port_t                 port_;

        public:

            TemplateTcpServer(net::address_t address, net::port_t port, net::SelectExtension& select, int interval = 2000);
            TemplateTcpServer(TemplateTcpServer&& channel);

            ~TemplateTcpServer() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int GetLeftTimeWork() const override;
            int Close() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            std::list<T>& GetClients();

        private:

            int checkConnect();
            int checkClients();

        };
    }
}

#include "template_tcp_server.tpp"

#endif
