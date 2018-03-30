#ifndef _MMXLST_LISTENER_H
#define _MMXLST_LISTENER_H

#include "mmxlib/net/portset.h"
#include "mmxlib/net/socket.h"
#include "mmxlib/net/select.h"

#include "mmxlib/ipc/pchannel.h"

#include "mmxlib/tools/timer.h"

#include "mmxlib/staff/ipstaffer.h"
#include "mmxlib/staff/udpstaffer.h"
#include "mmxlib/staff/datapacket.h"

#include "direction.h"


namespace mmxlst
{
    class Listener
    {
        mmx::net::PortSet& ports_;
        mmx::net::Socket socket_;
        mmx::net::Select select_;

        mmx::ipc::PipeChannel pipe_;

        mmx::tools::Timer timers_[2];
        mmx::tools::Timer& sock_timer_ = timers_[0];
        mmx::tools::Timer& pipe_timer_ = timers_[1];

        mmx::staff::DataPacket datapack_;
        Direction direction_;
        mmx::staff::UDPStaffer udp_sniffer_;
        mmx::staff::IPStaffer ip_sniffer_;

        mmx::net::timeout_t timeout_;

        mmx::net::address_t address_;
        unsigned char   channel_;

        unsigned short pack_id_;

        char buffer_[1600];


    public:

        Listener(mmx::net::address_t address, unsigned char channel, mmx::net::PortSet& ports);
        ~Listener();

        int Execute();

    private:
        int initialization();
        int checkConnection();
        int setTimeout();
        int waitEvents();
        int checkWrite();

        int readData();
        int writeData();
        int clear();

    };
}

#endif
