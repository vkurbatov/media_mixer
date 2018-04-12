#include "listener.h"

#include <cstring>

#include <netdb.h>  // SOCK_RAW, IPPROTO_UDP
#include <fcntl.h>  // O_NONBLOCK
#include <errno.h>
#include <sys/time.h>   // gettimeofday

#include "mmxlib/logs/log.h"
#include "mmxlib/headers/udp.h"
#include "mmxlib/headers/media.h"
#include "mmxlib/names.h"



#define MAX_DEFFERED_SIZE   100
#define DEFAULT_TIMEOUT     2000

namespace mmxlst
{

    Listener::Listener(mmx::net::address_t address, unsigned char channel, mmx::net::PortSet& ports) :
        ports_(ports),
        socket_(SOCK_RAW, IPPROTO_UDP),
        address_(address),
        channel_(channel),
        datapack_(0x10000),
        ip_sniffer_(packet_pool_),
        pack_id_(0),
        //sock_adp_(socket_),
        deffered_writer_(pipe_, MAX_DEFFERED_SIZE)
    {
        mmx::logs::logD("[%p] Listener::Listener(%d, %d, [ports])", this, address, channel);
    }

    Listener::~Listener()
    {
        mmx::logs::logD("[%p] Listener::~Listener(begin)", this);

        clear();

        mmx::logs::logD("[%p] Listener::~Listener(end)", this);

    }

    int Listener::Execute()
    {
        mmx::logs::logD("[%p] Listener::Execute(begin)", this);

        int rc = initialization();

        while(rc >= 0)
        {
            checkConnection();
            setTimeout();
            if (waitEvents() > 0)
            {
                readData();
                writeData();
            }
        }
        clear();

        mmx::logs::logD("[%p] Listener::Execute(stop)", this);
    }

    int Listener::initialization()
    {
        mmx::logs::logD("[%p] Listener::initialization(begin)", this);


        datapack_.BuildPacket(++pack_id_);
        sock_timer_.Start(0);
        pipe_timer_.Start(0);

        mmx::logs::logD("[%p] Listener::initialization(stop)", this);

        return 0;
    }

    int Listener::checkConnection()
    {
        int rc = 0;

        char pipe_name_[sizeof(MMX_LISTENER_CHANNEL_PATTERN)+4];

        std::sprintf(pipe_name_, MMX_LISTENER_CHANNEL_PATTERN, (int)channel_);

        // проверка сокетного соединения

        if (socket_.Handle() < 0)
        {
            if (sock_timer_.IsEnable())
            {
                rc = socket_.Open(0, 0, 0, 0, 0, O_NONBLOCK);

                if (rc >= 0)
                {
                    mmx::logs::logI("[%p] Listener::checkConnection() socket %d open success!", this, rc);
                    select_.Set(rc, mmx::net::S_EV_READ);
                }
                else
                {
                    mmx::logs::logE("[%p] Listener::checkConnection() socket open error = %d!", this, rc);
                }

                sock_timer_.Start(DEFAULT_TIMEOUT);
            }
        }

        // проверка pipe

        if (pipe_.Handle() < 0)
        {
            if (pipe_timer_.IsEnable())
            {
                rc = pipe_.Open(pipe_name_, O_RDWR | O_NONBLOCK);

                if (rc >= 0)
                {
                    mmx::logs::logI("[%p] Listener::checkConnection() pipe %d (%s) open success!", this, rc, pipe_name_);

                    checkWrite();

                }
                else
                {
                    mmx::logs::logE("[%p] Listener::checkConnection() pipe %s open error = %d!", this, pipe_name_, rc);
                }

                pipe_timer_.Start(DEFAULT_TIMEOUT);
            }
        }

    }

    int Listener::setTimeout()
    {
        int tout = mmx::net::INFINITE_TIMEOUT;

        // TODO: костыль... мне не нравится....

        if (socket_.Handle() < 0)
        {
            auto left = sock_timer_.Left();

            if (tout < 0 || tout > left)
            {
                tout = left < 0 ? 0 : left;
            }
        }


        if (pipe_.Handle() < 0)
        {
            auto left = pipe_timer_.Left();

            if (tout < 0 || tout > left)
            {
                tout = left < 0 ? 0 : left;
            }
        }

        timeout_ = tout;

        return timeout_;
    }

    int Listener::waitEvents()
    {

        int rc = 0;

        if (select_.IsEmpty())
        {
            mmx::tools::Timer::Sleep(timeout_);
        }
        else
        {

            rc = select_.Wait(timeout_);

            if (rc < 0)
            {
                if (rc == -ETIMEDOUT)
                {

                    mmx::logs::logW("[%p] Listener::waitEvents() select timeout rc = %d", this, rc);
                    rc = 0;
                }
                else
                {
                    mmx::logs::logE("[%p] Listener::waitEvents() select error rc = %d", this, rc);

                    clear();
                }
            }
        }

        return rc;
    }


    int Listener::checkWrite()
    {
        if (!datapack_.IsEmpty())
        {
            select_.Set(pipe_.Handle(), mmx::net::S_EV_WRITE);
        }

        return 0;
    }

    int Listener::readData()
    {
        int rc = 0;

        if (select_.IsRead(socket_.Handle()))
        {
            rc = socket_.Recv(buffer_, sizeof(buffer_));

            if (rc < 0)
            {

                switch(rc)
                {
                    case -EWOULDBLOCK:

                        mmx::logs::logW("[%p] Listener::readData() non-blocket forced reading socket %d. rc = %d", this, socket_.Handle(), rc);

                        break;
                    case -EINTR:

                        mmx::logs::logE("[%p] Listener::readData() Reading socket %d interrupted. rc = %d", this, socket_.Handle(), rc);

                        break;
                    default:

                        mmx::logs::logE("[%p] Listener::readData() socket =%d read error! rc = %d", this, socket_.Handle(), rc);

                        select_.Set(socket_.Handle());
                        socket_.Close();

                        break;
                }

            }
            else
            {

                int ret = rc;

                while (ret > 0)
                {

                    ret = ip_sniffer_.PutStream(buffer_, ret);


                    if (ip_sniffer_.IsComplete())
                    {

                        putPacket(*ip_sniffer_.GetPacket());

                        ip_sniffer_.Drop();

                    }

                    if (ret > 0)
                    {
                        ret = rc - ret;
                    }

                }

                checkWrite();

            }
        }

        return rc;
    }

    int Listener::writeData()
    {
        int rc = 0;

        if (select_.IsWrite(pipe_.Handle()))
        {
            // событие по наличию свободного канала

            if (datapack_.IsEmpty())
            {
                // ситуация непонятная. Сработали по записи, а данных в направлении нет...
                // распечатаем ошибку.

                mmx::logs::logE("[%p] Listener::writeData() No data for write!", this);

            }
            else
            {
                auto pack = datapack_.Header();

                // данные получены - нужно их пересылать (через механизм отложенной передачи)

                rc = deffered_writer_.Write((const char*)pack, pack->header.length);

                // ошибка передачи...

                if (rc < 0)
                {
                    switch (rc)
                    {
                        case -EWOULDBLOCK:

                            // ситуация нормальная, но опять спорная! почему сработал select????
                            // нужно распечатать

                            mmx::logs::logW("[%p] Listener::writeData() non-blocket forced writing pipe = \'%s\'. rc = %d", this, pipe_.Name(), rc);

                            break;

                        case -ENOBUFS:
                        case -EINTR:

                            mmx::logs::logW("[%p] Listener::writeData() writing pipe = \'%s\' interrupted. rc = %d", this, pipe_.Name(), rc);

                            break;
                        default:

                            mmx::logs::logE("[%p] Listener::writeData() writing pipe = \'%s\' error!! rc = %d", this, pipe_.Name(), rc);

                            select_.Set(pipe_.Handle());

                            pipe_.Close();

                            deffered_writer_.Drop();

                            break;
                    }

                }
                else
                {
                    mmx::logs::logI("[%p] Listener::writeData() pipe %s write %d bytes!", this, pipe_.Name(), rc);

                    // dir.Next();

                    datapack_.BuildPacket(++pack_id_);
                }
            }

            // если нет данных на передачу, то и сигналить о возможности не нужно

            if (datapack_.IsEmpty())
            {
                select_.Set(pipe_.Handle());
            }
        }

        return rc;
    }

    int Listener::clear()
    {
        select_.Reset();
        socket_.Close();
        pipe_.Close();
    }

    int Listener::putPacket(const mmx::sniffers::IIPPacket& packet)
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

                        auto block = datapack_.QueryBlock(size + sizeof(mmx::headers::MEDIA_HEADER));

                        if(block != nullptr)
                        {
                            mmx::headers::MEDIA_DATA& media =  *(mmx::headers::PMEDIA_DATA)block->data;

                            media.header.magic = mmx::headers::MEDIA_MAGIC;
                            media.header.length = size + sizeof(mmx::headers::MEDIA_HEADER);
                            media.header.addr_src = ::htonl(ip_header.src);
                            media.header.addr_dst = addr_dst;
                            media.header.port_src = ::htons(udp.port_src);
                            media.header.port_dst = port_dst;

                            timeval tv;

                            gettimeofday(&tv, 0);

                            media.header.timestamp = (tv.tv_sec % 86400) * 1000 + (tv.tv_usec / 1000);

                            std::copy(pyload, pyload + size, media.media);

                            rc = size;

                            datapack_.Commit();

                        }
                        else
                        {
                            datapack_.BuildPacket(pack_id_);
                        }
                    }

                }

            }
        }

        return rc;
    }
}
