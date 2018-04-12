#include "server.h"

#include <netdb.h>  // SOCK_RAW, IPPROTO_UDP
#include <fcntl.h>  // O_NONBLOCK
#include <errno.h>
#include <sys/time.h>   // gettimeofday

#include "mmxlib/logs/log.h"
#include "mmxlib/names.h"

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
        udp_socket_(mmx::net::Socket(SOCK_DGRAM, IPPROTO_UDP))

    {
        mmx::logs::logD("[%p] Server::Server(%d, %d:%d, %d:%d)", this, channel, tcp_address, tcp_port, udp_address, udp_port);
    }

    Server::~Server()
    {
        mmx::logs::logD("[%p] Server::~Listener(begin)", this);

        clear();

        mmx::logs::logD("[%p] Server::~Listener(end)", this);
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
                checkClients();
                readData();
                writeData();
            }
        }
        clear();

        mmx::logs::logD("[%p] Server::Execute(stop)", this);
    }

    int Server::initialization()
    {

    }

    int Server::checkConnection()
    {

    }

    int Server::setTimeout()
    {

    }

    int Server::waitEvents()
    {

    }

    int Server::checkAccept()
    {

    }

    int Server::checkClients()
    {

    }

    int Server::checkWrite()
    {

    }


    int Server::readData()
    {

    }

    int Server::writeData()
    {

    }

    int Server::clear()
    {
        select_.Reset();

        for (auto& c : clients_)
        {
            c.Socket().Close();
        }
        clients_.clear();

        tcp_socket_.Close();
        udp_socket_.Socket().Close();
        pipe_.Close();
    }


}
