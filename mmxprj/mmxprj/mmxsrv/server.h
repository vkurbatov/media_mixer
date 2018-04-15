#ifndef _MMXSRV_SERVER_H
#define _MMXSRV_SERVER_H

#include <list>
#include <vector>

#include "mmxlib/net/portset.h"
#include "mmxlib/net/socket.h"
#include "mmxlib/net/select_ex.h"

#include "mmxlib/ipc/pchannel.h"

#include "mmxlib/tools/timer.h"
#include "mmxlib/tools/iosockadp.h"
#include "mmxlib/tools/deffwriter.h"
#include "mmxlib/tools/deffersock.h"


#include "mmxlib/headers/order645_2.h"
#include "mmxlib/headers/si.h"

#include "mmxlib/sniffers/dpsniffer.h"


namespace mmxsrv
{
    class Server
    {

        //mmx::net::Socket udp_socket_;
        mmx::net::Socket tcp_socket_;
        mmx::net::Socket udp_socket_;
        mmx::net::SelectExtension select_;

        mmx::ipc::PipeChannel pipe_;

        mmx::tools::Timer timers_[3];

        mmx::tools::Timer& tcp_timer_ = timers_[0];
        mmx::tools::Timer& udp_timer_ = timers_[1];
        mmx::tools::Timer& pipe_timer_ = timers_[2];

        mmx::tools::SocketIOAdapter udp_adapter_;
        mmx::tools::DeferredWriter udp_writer_;

        mmx::net::timeout_t timeout_;

        mmx::net::address_t udp_address_;
        mmx::net::port_t udp_port_;

        mmx::net::address_t tcp_address_;
        mmx::net::port_t tcp_port_;

        mmx::sniffers::DataPackSniffer dp_sniffer_;

        unsigned char   channel_;

        unsigned short pack_id_;

        bool data_write_;

        std::list<mmx::tools::DeferredSocket>   clients_;
        std::vector<mmx::tools::DeferredSocket*> remove_list_;

        union
        {
            mmx::headers::DATA_PACK data_pack_;
            char input_[mmx::headers::MAX_PACKET_SIZE + 1];
        };

    public:

        Server(unsigned char channel = 1,
               mmx::net::address_t tcp_address = 0,
               mmx::net::port_t tcp_port = 0,
               mmx::net::address_t udp_address = 0,
               mmx::net::port_t udp_port = 0);
        ~Server();

        int Execute();

    private:

        // методы конвеера

        int initialization();
        int checkConnection();
        int setTimeout();
        int waitEvents();
        int checkAccept();
        int checkClients();
        int udpWrite(const mmx::headers::SANGOMA_PACKET* sangoma = nullptr);
        int tcpWrite(const mmx::headers::MEDIA_INFO_PACKET* media = nullptr);

        int readData();
        int processData(const mmx::headers::DATA_PACK* data_pack);

        int writeData(const mmx::headers::MEDIA_INFO_PACKET* media = nullptr);
        int removeClients();
        int clear();

    };
}

#endif
