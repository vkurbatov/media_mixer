#ifndef _MMXSRV_SERVER_H
#define _MMXSRV_SERVER_H

#include <list>

#include "mmxlib/net/portset.h"
#include "mmxlib/net/socket.h"
#include "mmxlib/net/select.h"

#include "mmxlib/ipc/pchannel.h"

#include "mmxlib/tools/timer.h"
#include "mmxlib/tools/iosockadp.h"
#include "mmxlib/tools/deffwriter.h"
#include "mmxlib/tools/deffersock.h"

#include "mmxlib/data/datapacket.h"

#include "mmxlib/headers/order645_2.h"

#include "mmxlib/sniffers/dpsniffer.h"


namespace mmxsrv
{
    class Server
    {

        //mmx::net::Socket udp_socket_;
        mmx::net::Socket tcp_socket_;
        mmx::net::Select select_;

        mmx::ipc::PipeChannel pipe_;

        mmx::tools::Timer timers_[2];
        mmx::tools::Timer& sock_timer_ = timers_[0];
        mmx::tools::Timer& pipe_timer_ = timers_[1];

        mmx::tools::DeferredSocket udp_socket_;

        mmx::data::DataPacket datapack_;

        mmx::net::timeout_t timeout_;

        mmx::net::address_t udp_address_;
        mmx::net::port_t udp_port_;

        mmx::net::address_t tcp_address_;
        mmx::net::port_t tcp_port_;

        mmx::sniffers::DataPackSniffer dp_sniffer;

        unsigned char   channel_;

        unsigned short pack_id_;

        std::list<mmx::tools::DeferredSocket>   clients_;

    public:

        Server(unsigned char channel = 1,
               mmx::net::address_t tcp_address = 0,
               mmx::net::port_t tcp_port = 0,
               mmx::net::address_t udp_address = 0,
               mmx::net::port_t udp_port = 0);
        ~Server();

        int Execute();

    private:

        int initialization();
        int checkConnection();
        int setTimeout();
        int waitEvents();
        int checkAccept();
        int checkClients();
        int checkWrite();

        int readData();
        int writeData();
        int clear();

    };
}

#endif
