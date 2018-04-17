#include "sangoma_server.h"

#include <fcntl.h>  // O_NONBLOCK
#include <netdb.h>
#include <errno.h>

namespace mmx
{
    namespace tools
    {

        SangomaServer::SangomaServer(net::address_t address, net::port_t port, net::SelectExtension& select, int interval) :
            address_(address),
            port_(port),
            select_(select),
            interval_(interval),
            socket_(SOCK_STREAM, IPPROTO_TCP)
        {
            timer_.Start(0);
        }

        SangomaServer::SangomaServer(SangomaServer&& channel) :
            address_(channel.address_),
            port_(channel.port_),
            socket_(std::move(channel.socket_)),
            clients_(std::move(channel.clients_)),
            select_(channel.select_),
            interval_(channel.interval_),
            timer_(std::move(channel.timer_))
        {

        }

        SangomaServer::~SangomaServer()
        {
            Close();
        }

        int SangomaServer::Dispatch()
        {

            checkConnect();
            checkClients();

        }

        int SangomaServer::GetLeftTimeWork() const
        {
            return socket_.Handle() < 0 && timer_.IsStarted()
                    ? timer_.Left()
                    : mmx::net::INFINITE_TIMEOUT;
        }

        int SangomaServer::Close()
        {
            socket_.Close();
        }

        bool SangomaServer::IsDown() const
        {
            return socket_.Handle() < 0;
        }

        std::list<SangomaClient>& SangomaServer::GetClients()
        {
            return clients_;
        }

        int SangomaServer::checkConnect()
        {
            int rc = 0;

            if (timer_.IsEnable())
            {

                if (socket_.Handle() < 0)
                {
                    rc = socket_.Open(address_, port_, 0, 0, 4, O_NONBLOCK);

                    if (rc > 0)
                    {

                        select_.SetRead(rc);

                    }
                }

                timer_.Start(interval_);
            }

            return rc;
        }

        int SangomaServer::checkClients()
        {
            int rc = 0;

            int fd = socket_.Handle();

            if (fd >= 0)
            {

                if (select_.IsRead(fd))
                {

                    net::Socket client(SOCK_STREAM, IPPROTO_TCP);

                    rc = client.Accept(socket_, O_NONBLOCK);

                    if (rc >= 0)
                    {
                        clients_.push_back(SangomaClient(std::move(client), select_));
                    }
                    else
                    {
                        if (rc != EAGAIN)
                        {
                            Close();
                        }
                    }

                }

            }
            return rc;
        }

    }
}
