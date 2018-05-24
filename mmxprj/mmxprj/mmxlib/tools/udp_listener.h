#ifndef _MMX_TOOLS_UDP_LISTENER_H
#define _MMX_TOOLS_UDP_LISTENER_H

#include "ichanneldispatcher.h"
#include "data/idpwriter.h"
#include "net/portset.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "sniffers/ipsniffer.h"
#include "timer.h"

namespace mmx
{
    namespace tools
    {
        class UdpListener : public IChannelDispatcher
        {

            net::Socket                 socket_;
            net::SelectExtension&       select_;
            const net::PortSet&         ports_;
            data::IDataPacketWriter&    dp_writer_;
            sniffers::IPSniffer         ip_sniffer_;
            Timer                       timer_;

            unsigned int                address_;
            int                         interval_;
            std::vector<char>           data_;
            unsigned short              media_pack_id_;
            int                         blocks_;


        public:

            UdpListener(const net::PortSet& ports, unsigned int address, data::IDataPacketWriter& dp_writer, sniffers::IPPacketPool& packet_pool, net::SelectExtension& select, int interval = 2000);
            UdpListener(UdpListener&& listener);
            ~UdpListener();

            // IChannelDispatcher interface
            int Dispatch(dispatch_flags_t dispatch, void *context = nullptr);
            int QueryOrderTimeout() const;
            int Close();
            bool IsDown() const;
            bool IsReadyData() const;

        private:

            int init();
            int processData();
            int timerProc();

            int putPacket(const mmx::sniffers::IIPPacket& packet);

        };
    }
}

#endif
