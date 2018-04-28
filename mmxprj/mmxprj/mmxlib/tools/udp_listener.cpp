#include "udp_listener.h"

#include "headers/media.h"
#include "headers/udp.h"


#include <errno.h>
#include <netdb.h>      // IPPROTO_UDP
#include <fcntl.h>      // O_NONBLOCK
#include <sys/time.h>   // gettimeofday


namespace mmx
{
    namespace tools
    {
        UdpListener::UdpListener(const net::PortSet& ports,
                                 data::IDataPacketWriter& dp_writer,
                                 sniffers::IPPacketPool& packet_pool,
                                 net::SelectExtension& select,
                                 int interval) :
            ports_(ports),
            dp_writer_(dp_writer),
            ip_sniffer_(packet_pool),
            select_(select),
            socket_(SOCK_RAW, IPPROTO_UDP),
            interval_(interval),
            data_(1600),
            media_pack_id_(0),
            blocks_(0)

        {
            //dp_writer_.BuildPacket(++pack_id_);
            timer_.Start(0);
        }

        UdpListener::UdpListener(UdpListener&& listener) :
           ports_(listener.ports_),
           dp_writer_(listener.dp_writer_),
           ip_sniffer_(std:: move(listener.ip_sniffer_)),
           timer_(std::move(listener.timer_)),
           select_(listener.select_),
           socket_(std::move(listener.socket_)),
           interval_(listener.interval_),
           data_(std::move(listener.data_)),
           media_pack_id_(listener.media_pack_id_),
           blocks_(0)
        {

        }

        UdpListener::~UdpListener()
        {

        }

        int UdpListener::Dispatch(dispatch_flags_t dispatch, void *context)
        {
            int rc = -EINVAL;

            switch(dispatch)
            {
                case DISPATCH_INIT:
                    rc = init();
                    break;
                case DISPATCH_IO:
                    rc = processData();
                    break;
                case DISPATCH_TIMER:
                    rc = timerProc();
                    break;
            }

            return rc;
        }

        int UdpListener::QueryOrderTimeout() const
        {
            return socket_.Handle() < 0 && timer_.IsStarted()
                    ? timer_.Left()
                    : mmx::net::INFINITE_TIMEOUT;
        }

        int UdpListener::Close()
        {
            int rc = 0;

            if (socket_.Handle() > 0)
            {
                select_.Set(socket_.Handle());
                rc = socket_.Close();
            }

            return rc;
        }

        bool UdpListener::IsDown() const
        {
            return socket_.Handle() < 0;
        }

        bool UdpListener::IsReadyData() const
        {
            return blocks_ > 0;
        }

        int UdpListener::init()
        {
            timerProc();
        }

        int UdpListener::processData()
        {
            int rc = 0;

            int fd = socket_.Handle();

            if (fd >= 0)
            {
                if (select_.IsRead(fd))
                {

                    char* buffer = data_.data();

                    rc = socket_.Recv(buffer, data_.size());

                    if (rc > 0)
                    {
                        int ret = 1;

                        int off = 0;

                        while (ret > 0 && off < rc)
                        {

                            ret = ip_sniffer_.PutStream(buffer + off, rc - off);

                            if (ip_sniffer_.IsComplete())
                            {

                                putPacket(*ip_sniffer_.GetPacket());

                                ip_sniffer_.Drop();

                            }

                            off += ret > 0 ? ret : 0;

                        }

                    }
                    else
                    {
                        switch (rc)
                        {
                            case -EAGAIN:

                                // ничего не делаем

                                break;
                            default:

                                Close();
                                fd = -1;

                        }
                    }

                }
            }

            return rc;
        }

        int UdpListener::timerProc()
        {
            int rc = 0;

            if (timer_.IsEnable())
            {

                if (socket_.Handle() < 0)
                {
                    rc = socket_.Open(0, 0, 0, 0, 0, O_NONBLOCK);

                    if (rc >= 0)
                    {
                        select_.SetRead(rc);
                    }
                }

                timer_.Start(interval_);
            }

            return rc;
        }

        int UdpListener::putPacket(const sniffers::IIPPacket& packet)
        {
            int rc = -EINVAL;

            if (packet.Header() != nullptr)
            {
                rc = -EPROTO;

                auto& ip_header = *packet.Header();
                auto pyload = (const char*)packet.Pyload();
                auto size = packet.Size();

                if (ip_header.protocol == IPPROTO_UDP)
                {
                    mmx::headers::UDPHEADER& udp = *(mmx::headers::PUDPHEADER)pyload;

                    unsigned int addr_dst = ::htonl(ip_header.dest);
                    unsigned short port_dst = ::htons(udp.port_dst);

                    rc = -EBADMSG;

                    if (::ntohs(udp.length) == size)
                    {
                        pyload += sizeof(mmx::headers::UDPHEADER);
                        size -= sizeof(mmx::headers::UDPHEADER);

                        rc= -EADDRNOTAVAIL;

                        if (ports_[port_dst] && (address_ == INADDR_ANY || address_ == addr_dst))
                        {
                            rc = -ENOMEM;

                            auto block = dp_writer_.QueryBlock(size + sizeof(mmx::headers::MEDIA_HEADER));

                            if(block != nullptr)
                            {
                                mmx::headers::MEDIA_DATA& media =  *(mmx::headers::PMEDIA_DATA)block->data;

                                media.header.magic = mmx::headers::MEDIA_MAGIC;
                                media.header.length = size + sizeof(mmx::headers::MEDIA_HEADER);
                                media.header.net_points.source.address = ::htonl(ip_header.src);
                                media.header.net_points.destination.address = addr_dst;
                                media.header.net_points.source.port = ::htons(udp.port_src);
                                media.header.net_points.destination.port = port_dst;
                                media.header.packet_id = ++media_pack_id_;

                                timeval tv;

                                gettimeofday(&tv, 0);

                                media.header.timestamp = (tv.tv_sec % 86400) * 1000 + (tv.tv_usec / 1000);

                                std::copy(pyload, pyload + size, media.media);

                                rc = size;

                                blocks_ += (int)(dp_writer_.Commit() > 0);


                            }
                            else
                            {
                                dp_writer_.BuildPacket(0);
                                blocks_ = 0;
                            }
                        }

                    }

                }
            }

            return rc;
        }

    }
}
