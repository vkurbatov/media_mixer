#include "orm_server.h"

#include <fcntl.h>  // O_NONBLOCK
#include <netdb.h>
#include <errno.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("OrmServer", msg)

namespace mmx
{
    namespace tools
    {

        OrmServer::OrmServer(net::address_t address, net::port_t port, net::SelectExtension& select, int interval) :
            address_(address),
            port_(port),
            select_(select),
            interval_(interval),
            socket_(SOCK_STREAM, IPPROTO_TCP)
        {
            timer_.Start(0);
        }

        OrmServer::OrmServer(OrmServer&& channel) :
            address_(channel.address_),
            port_(channel.port_),
            socket_(std::move(channel.socket_)),
            clients_(std::move(channel.clients_)),
            select_(channel.select_),
            interval_(channel.interval_),
            timer_(std::move(channel.timer_))
        {

        }

        OrmServer::~OrmServer()
        {
            Close();
        }

        int OrmServer::Dispatch(dispatch_flags_t dispatch, void* context)
        {

            checkConnect();
            return checkClients();

        }

        int OrmServer::QueryOrderTimeout() const
        {
            return socket_.Handle() < 0 && timer_.IsStarted()
                    ? timer_.Left()
                    : mmx::net::INFINITE_TIMEOUT;
        }

        int OrmServer::Close()
        {
            socket_.Close();
        }

        bool OrmServer::IsDown() const
        {
            return socket_.Handle() < 0;
        }

        bool OrmServer::IsReadyData() const
        {
            return false;
        }


        std::list<OrmClient>& OrmServer::GetClients()
        {
            return clients_;
        }

        int OrmServer::checkConnect()
        {
            int rc = 0;

            if (timer_.IsEnable())
            {

                if (socket_.Handle() < 0)
                {
                    rc = socket_.Open(address_, port_, 0, 0, 4, O_NONBLOCK);

                    if (rc > 0)
                    {
                        DLOGI(LOG_BEGIN("checkConnect(): orm server %d:%d create success, sock = %d"), address_, port_, rc);
                        select_.SetRead(rc);

                    }
                    else
                    {
                        DLOGW(LOG_BEGIN("checkConnect(): orm server %d:%d is not create, rc = %d"), address_, port_, rc);
                    }
                }

                timer_.Start(interval_);
            }

            return rc;
        }

        int OrmServer::checkClients()
        {
            int rc = 0;

            int fd = socket_.Handle();

            if (fd >= 0)
            {

                if (select_.IsRead(fd))
                {

                    net::Socket client(SOCK_STREAM, IPPROTO_TCP);

                    rc = client.Accept(socket_);

                    if (rc >= 0)
                    {

                        DLOGI(LOG_BEGIN("checkClients(): orm client %d:%d accept connection success, sock = %d"), client.RemoteAddress(), client.RemotePort(), rc);

                        clients_.push_back(OrmClient(std::move(client), select_));
                    }
                    else
                    {
                        DLOGE(LOG_BEGIN("checkClients(): orm client accept connection is failed, rc = %d"), rc);

                        if (rc != -EAGAIN)
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
