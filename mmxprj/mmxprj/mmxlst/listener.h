#ifndef _MMXLST_LISTENER_H
#define _MMXLST_LISTENER_H

#include "mmxlib/net/portset.h"
#include "mmxlib/net/socket.h"
#include "mmxlib/net/select_ex.h"

#include "mmxlib/data/datapacket.h"

#include "mmxlib/sniffers/ipsniffer.h"

#include "mmxlib/tools/pipe_output_channel.h"
#include "mmxlib/tools/udp_listener.h"


namespace mmxlst
{
    typedef struct _LISTENER_CONFIG
    {
        mmx::net::address_t address;
        unsigned char       channel;
        int                 interval;
    }LISTENER_CONFIG, *PLISTENER_CONFIG;

    class Listener
    {
        mmx::net::SelectExtension           select_;
        mmx::sniffers::IPPacketPool         packet_pool_;
        mmx::tools::PipeOutputChannel       output_channel_;
        mmx::tools::UdpListener             udp_listener_;

        LISTENER_CONFIG                     config_;

    public:

        Listener(const LISTENER_CONFIG& config, const mmx::net::PortSet& ports);
        ~Listener();
        int Execute();

    private:
        void init();
        int getTimeouts();
        void dispatchAll(mmx::tools::dispatch_flags_t dispatch);
        void processData();
        void closeAll();

    };

    /*
    class Listener
    {
        mmx::net::PortSet& ports_;
        mmx::net::Socket socket_;
        mmx::net::SelectExtension select_;

        mmx::ipc::PipeChannel pipe_;

        mmx::tools::Timer timers_[2];
        mmx::tools::Timer& sock_timer_ = timers_[0];
        mmx::tools::Timer& pipe_timer_ = timers_[1];

        mmx::tools::DeferredWriter deffered_writer_;

        mmx::data::DataPacket datapack_;

        mmx::sniffers::IPPacketPool packet_pool_;
        mmx::sniffers::IPSniffer ip_sniffer_;

        mmx::net::timeout_t timeout_;

        mmx::net::address_t address_;
        unsigned char   channel_;

        unsigned short pack_id_;
        unsigned short media_pack_id_;

        char buffer_[mmx::sniffers::DEFAULT_MTU_SIZE * 10];


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
        int checkPipe();
        int writeData();
        int clear();

        int putPacket(const mmx::sniffers::IIPPacket& packet);

    };
    */
}

#endif
