#include "server.h"

#include <netdb.h>  // SOCK_RAW, IPPROTO_UDP
#include <fcntl.h>  // O_NONBLOCK
#include <errno.h>
#include <sys/time.h>   // gettimeofday

#include "mmxlib/logs/log.h"
#include "mmxlib/names.h"

#include "mmxlib/data/dpreader.h"

#define DEFAULT_TIMEOUT 2000

namespace mmxsrv
{
    Server::Server(unsigned char channel,
           mmx::net::address_t tcp_address,
           mmx::net::port_t tcp_port,
           mmx::net::address_t udp_address,
           mmx::net::port_t udp_port) :
        channel_(channel),
        tcp_address_(tcp_address),
        tcp_port_(tcp_port),
        udp_address_(udp_address),
        udp_port_(udp_port),
        pack_id_(0),
        tcp_socket_(SOCK_STREAM, IPPROTO_TCP),
        udp_socket_(SOCK_DGRAM, IPPROTO_UDP),
        udp_adapter_(udp_socket_),
        udp_writer_(udp_adapter_),
        data_write_(false)

    {
        mmx::logs::logD("[%p] Server::Server(%d, %d:%d, %d:%d)", this, channel, tcp_address, tcp_port, udp_address, udp_port);
    }

    Server::~Server()
    {
        mmx::logs::logD("[%p] Server::~Server(begin)", this);

        clear();

        mmx::logs::logD("[%p] Server::~Server(end)", this);
    }

    int Server::Execute()
    {
        mmx::logs::logD("[%p] Server::Execute(begin)", this);

        int rc = initialization();

        while(rc >= 0)
        {
            checkConnection();
            setTimeout();
            if (waitEvents() > 0)
            {
                checkAccept();
                readData();
                checkClients();

                if (!data_write_)
                {
                    writeData();
                }

            }
        }
        clear();

        mmx::logs::logD("[%p] Server::Execute(stop)", this);
    }

    int Server::initialization()
    {
        mmx::logs::logD("[%p] Server::initialization(begin)", this);


        tcp_timer_.Start(0);
        udp_timer_.Start(0);
        pipe_timer_.Start(0);

        mmx::logs::logD("[%p] Server::initialization(stop)", this);

    }

    int Server::checkConnection()
    {
        int rc = 0;

        char pipe_name_[sizeof(MMX_SERVER_CHANNEL_PATTERN)+4];

        std::sprintf(pipe_name_, MMX_SERVER_CHANNEL_PATTERN, (int)channel_);

        // сперва открываем канал

        if (pipe_.Handle() <= 0)
        {
            if (pipe_timer_.IsEnable())
            {

                rc = pipe_.Open(pipe_name_, O_RDONLY | O_NONBLOCK, 0777);

                if (rc >= 0)
                {

                    mmx::logs::logI("[%p] Server::checkConnection() pipe %d (%s) open success!", this, rc, pipe_name_);
                    select_.SetRead(rc);

                }
                else
                {

                    mmx::logs::logE("[%p] Server::checkConnection() pipe %s open error = %d!", this, pipe_name_, rc);

                }

            }

        }

        // создаем tcp-соединение

        if (tcp_address_ != INADDR_NONE && tcp_port_ != 0)
        {
            if (tcp_socket_.Handle() < 0)
            {
                if (tcp_timer_.IsEnable())
                {
                    rc = tcp_socket_.Open(tcp_address_, tcp_port_, 0, 0, 4, O_NONBLOCK);

                    if (rc >= 0)
                    {

                        mmx::logs::logI("[%p] Listener::checkConnection() tcp_server %s:%d create success fd = %d!", this, mmx::net::Socket::AtoS(tcp_address_), tcp_port_,rc);
                        select_.SetRead(rc);

                    }
                    else
                    {
                        mmx::logs::logE("[%p] Listener::checkConnection() tcp_server %s:%d create error = %d!", this, mmx::net::Socket::AtoS(tcp_address_), tcp_port_,rc);
                    }

                    tcp_timer_.Start(DEFAULT_TIMEOUT);
                }
            }
        }

        // создаем udp-соединение

        if (udp_address_ != INADDR_NONE && udp_port_ != 0)
        {
            if (udp_socket_.Handle() < 0)
            {
                if (udp_timer_.IsEnable())
                {
                    rc = udp_socket_.Open(0, 0, udp_address_, udp_port_, 0, O_NONBLOCK);

                    if (rc >= 0)
                    {

                        mmx::logs::logI("[%p] Listener::checkConnection() udp_server %s:%d create success fd = %d!", this, mmx::net::Socket::AtoS(udp_socket_.LocalAddress()), udp_socket_.LocalPort(),rc);
                        select_.SetRead(rc);

                    }
                    else
                    {
                        mmx::logs::logE("[%p] Listener::checkConnection() udp_server %s:%d create error = %d!", this, mmx::net::Socket::AtoS(udp_socket_.LocalAddress()), udp_socket_.LocalPort(),rc);
                    }

                    udp_timer_.Start(DEFAULT_TIMEOUT);
                }
            }
        }

        return rc;

    }

    int Server::setTimeout()
    {
        int tout = mmx::net::INFINITE_TIMEOUT;

        // TODO: костыль... мне не нравится....


        if (pipe_.Handle() < 0)
        {
            auto left = pipe_timer_.Left();

            if (tout < 0 || tout > left)
            {
                tout = left < 0 ? 0 : left;
            }
        }

        if (tcp_address_ != INADDR_NONE && tcp_port_ != 0)
        {
            if (tcp_socket_.Handle() < 0)
            {
                auto left = tcp_timer_.Left();

                if (tout < 0 || tout > left)
                {
                    tout = left < 0 ? 0 : left;
                }
            }
        }

        if (udp_address_ != INADDR_NONE && udp_port_ != 0)
        {
            if (udp_socket_.Handle() < 0)
            {
                auto left = udp_timer_.Left();

                if (tout < 0 || tout > left)
                {
                    tout = left < 0 ? 0 : left;
                }
            }
        }

        timeout_ = tout;

        return timeout_;
    }

    int Server::waitEvents()
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

    int Server::checkAccept()
    {

        int rc = 0;

        if (tcp_socket_.Handle() >= 0)
        {
            if (select_.IsRead(tcp_socket_.Handle()))
            {
                //  пришел запрос на подключение

                mmx::net::Socket cli(tcp_socket_, O_NONBLOCK);

                rc = cli.Handle();

                if (rc >= 0)
                {

                    mmx::logs::logI("[%p] Server::checkAccept() new client %s:%d connected success, fd = %s!", this, mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), rc);

                    // селектируем чтение, это нужно для детекции факта отключения клиента

                    select_.SetRead(rc);
                    clients_.push_back(std::move(cli));

                }
                else
                {

                    mmx::logs::logE("[%p] Server::checkAccept() client connected error, rc = %d!", this, rc);

                }

            }
        }

        return rc;

    }

    int Server::checkClients()
    {

        int rc = 0;

        remove_list_.clear();

        for (auto& c : clients_)
        {
            mmx::net::Socket& cli = c.Socket();
            int fd = cli.Handle();

            if (select_.IsRead(fd))
            {


                static char dummy[1600];

                // сработали по чтению, скорее всего это разрыв соединения

                rc = cli.Recv(dummy, sizeof(dummy));

                if (rc <= 0)
                {
                    // клиент разорвал соединение

                    switch (rc)
                    {

                        case -EAGAIN:

                            mmx::logs::logW("[%p] Server::checkClients() client %s:%d non blocked read, rc = %d!", this, mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), fd);

                            break;
                        default:

                            if (rc == 0)
                            {

                                mmx::logs::logI("[%p] Server::checkClients() client %s:%d disconnect, fd = %d!", this, mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), cli.Handle());

                            }
                            else
                            {

                                mmx::logs::logI("[%p] Server::checkClients() client %s:%d read error, rc = %d!", this, mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), rc);

                            }

                            remove_list_.push_back(&c);

                            break;
                    }

                }
                else
                {
                    mmx::logs::logI("[%p] Server::checkClients() read from client %s:%d %d dummy bytes!", this, mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), rc);
                }

            }

        }

        removeClients();

        return rc;

    }


    int Server::tcpWrite(const mmx::headers::ORM_INFO_PACKET* media)
    {

        int rc = 0;

        static mmx::headers::ORDER_645_2_PACKET out;

        const void *data = media == nullptr ? nullptr : &media->header.order_header;
        int len = data == nullptr ? 0 : mmx::headers::ORDER_645_2_PACKET_SIZE;

        for (auto& c : clients_)
        {

            mmx::net::Socket& cli = c.Socket();
            mmx::tools::DeferredWriter& writer = c.Writter();

            int fd = cli.Handle();

            if (data != nullptr || select_.IsWrite(fd))
            {

                rc = writer.Write(data, len);

                if (rc <= 0 && rc != -EAGAIN)
                {
                     mmx::logs::logI("[%p] Server::tcpWrite() write to client %s:%d error, rc = %d!", this, mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), rc);

                     remove_list_.push_back(&c);

                }
                else
                {
                    mmx::logs::logD("[%p] Server::tcpWrite() write to client %s:%d %d bytes", this, mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), rc);

                    if (!writer.IsEmpty())
                    {
                        select_.SetWrite(fd);
                    }
                    else
                    {
                        select_.ClrWrite(fd);
                    }
                }

            }

        }

        removeClients();

        return rc;

    }

    int Server::udpWrite(const mmx::headers::SANGOMA_RAW_PACKET* sangoma)
    {

        int rc = 0;

        int len = sangoma == nullptr ? 0 : sizeof(mmx::headers::SANGOMA_RAW_PACKET);

        int fd = udp_socket_.Handle();

        if (sangoma != nullptr || select_.IsWrite(fd))
        {

            rc = udp_writer_.Write(sangoma, len);

            if (rc <= 0 && rc != -EAGAIN)
            {
                 mmx::logs::logI("[%p] Server::udpWrite() write to client %s:%d error, rc = %d!", this, mmx::net::Socket::AtoS(udp_socket_.RemoteAddress()), udp_socket_.RemotePort(), rc);

                 select_.Set(fd);
                 udp_socket_.Close(fd);

            }
            else
            {
                mmx::logs::logD("[%p] Server::udpWrite() write to client %s:%d %d bytes", this, mmx::net::Socket::AtoS(udp_socket_.RemoteAddress()), udp_socket_.RemotePort(), rc);

                if (!udp_writer_.IsEmpty())
                {
                    select_.SetWrite(fd);
                }
                else
                {
                    select_.ClrWrite(fd);
                }
            }

        }

        return rc;

    }


    int Server::readData()
    {

        int rc = 0;

        data_write_ = false;

        if (select_.IsRead(pipe_.Handle()))
        {

            rc = pipe_.Read(input_, sizeof(input_));

            if (rc < 0)
            {

                switch(rc)
                {
                    case -EWOULDBLOCK:

                        mmx::logs::logW("[%p] Server::readData() non-blocket forced reading pipe %s. rc = %d", this, pipe_.Name(), rc);

                        break;
                    case -EINTR:

                        mmx::logs::logE("[%p] Listener::readData() Reading pipe %s interrupted. rc = %d", this, pipe_.Name(), rc);

                        break;
                    default:

                        mmx::logs::logE("[%p] Listener::readData() pipe %s read error! rc = %d", this, pipe_.Name(), rc);

                        select_.Set(pipe_.Handle());
                        pipe_.Close();

                        break;
                }

            }
            else
            {

                int ret = rc;

                while (ret > 0)
                {

                    ret = dp_sniffer_.PutStream(input_, ret);


                    if (dp_sniffer_.IsComplete())
                    {

                        processData(dp_sniffer_.GetDataPacket());

                        dp_sniffer_.Drop();

                    }

                    if (ret > 0)
                    {
                        ret = rc - ret;
                    }

                }

            }
        }

        return rc;

    }

    int Server::processData(const mmx::headers::DATA_PACK* data_pack)
    {

        int rc = 0;

        mmx::data::DataPacketReader reader(&data_pack->header);

        auto block = reader.GetBlock();

        while (block != nullptr)
        {

            writeData((const mmx::headers::ORM_INFO_PACKET*)block->data);

            block = reader.GetBlock();
        }

        return rc;

    }

    int Server::writeData(const mmx::headers::ORM_INFO_PACKET* media)
    {

        int rc = 0;

        static mmx::headers::SANGOMA_RAW_PACKET sangoma_a = { 0 };
        static mmx::headers::SANGOMA_RAW_PACKET sangoma_b = { 0 };

        if (media != nullptr)
        {

            bool comb = media->header.order_header.mcl_a != media->header.order_header.mcl_b;

            sangoma_a.header.packet_id = ++sangoma_b.header.packet_id;
            sangoma_a.header.lid = media->header.order_header.mcl_a;
            sangoma_a.header.length = media->header.size_a;

            if (comb)
            {
                sangoma_b.header.packet_id++;
                sangoma_b.header.lid = media->header.order_header.mcl_b;
                sangoma_b.header.length = media->header.size_b;
            }

            for (int i = 0; i < mmx::headers::SI_MAX_PYLOAD_SIZE; i++)
            {

                sangoma_a.data[i] = i < sangoma_a.header.length ? media->data[i << (int)comb] : 0;

                if (comb)
                {
                    sangoma_b.data[i] = i < sangoma_b.header.length ? media->data[i * 2 + 1] : 0;
                }

            }

            udpWrite(&sangoma_a);

            if (comb)
            {
                udpWrite(&sangoma_b);
            }

        }
        else
        {
            udpWrite(nullptr);
        }

        tcpWrite(media);

        data_write_ = true;

        return rc;

    }

    int Server::removeClients()
    {
        int rc = 0;

        for (auto& p : remove_list_)
        {

            select_.Set(p->Socket().Handle());

            clients_.remove(*p);

            rc++;
        }

        remove_list_.clear();

        return rc;
    }

    int Server::clear()
    {
        select_.Reset();


        clients_.clear();

        tcp_socket_.Close();
        udp_socket_.Close();
        pipe_.Close();
    }


}
