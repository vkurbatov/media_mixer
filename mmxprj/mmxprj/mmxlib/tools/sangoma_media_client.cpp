#include "sangoma_media_client.h"

#include <cstring>

#include "headers/si.h"

#include <errno.h>
#include <netdb.h>  // SOCK_DGRAM
#include <fcntl.h>  // O_NONBLOCK

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("SangomaMediaClient", msg)

namespace mmx
{
    namespace tools
    {
        SangomaMediaClient::SangomaMediaClient(net::address_t address, net::port_t port, net::SelectExtension& select, int interval) :
            address_(address),
            port_(port),
            socket_(SOCK_DGRAM, IPPROTO_UDP),
            select_(select),
            adapter_(socket_),
            writer_(adapter_),
            data_(sizeof(headers::SANGOMA_PACKET)),
            interval_(interval)
        {

            std::memset(&sangoma_, 0, sizeof(sangoma_));

        }

        SangomaMediaClient::SangomaMediaClient(SangomaMediaClient&& channel) :
            address_(channel.address_),
            port_(channel.port_),
            socket_(std::move(channel.socket_)),
            select_(channel.select_),
            adapter_(socket_),
            writer_(adapter_),
            data_(std::move(channel.data_)),
            timer_(std::move(channel.timer_))
        {

        }

        SangomaMediaClient::~SangomaMediaClient()
        {
            Close();
        }

        int SangomaMediaClient::Dispatch(dispatch_flags_t dispatch, void* context)
        {
            int rc = 0;

            checkConnect();
            checkData();

            return rc;
        }

        int SangomaMediaClient::QueryOrderTimeout() const
        {
            return socket_.Handle() < 0 && timer_.IsStarted()
                    ? timer_.Left()
                    : mmx::net::INFINITE_TIMEOUT;
        }

        int SangomaMediaClient::Close()
        {
            writer_.Drop();

            if (socket_.Handle() >= 0)
            {

                select_.Set(socket_.Handle());
                socket_.Close();

            }
        }

        bool SangomaMediaClient::IsDown() const
        {
            return socket_.Handle() < 0;
        }

        bool SangomaMediaClient::IsReadyData() const
        {
            return false;
        }

        int SangomaMediaClient::PutData(const mmx::headers::ORM_INFO_PACKET& orm_info)
        {

            int comb = (int)(orm_info.header.order_header.mcl_a != orm_info.header.order_header.mcl_b);

            for (int i = 0; i < 1 + comb; i++)
            {
                sangoma_.header.packet_id++;
                sangoma_.header.lid = *(&orm_info.header.order_header.mcl_a + i);
                sangoma_.header.length = *(&orm_info.header.size_a + i);

                for (int j = 0; j < mmx::headers::SI_MAX_PYLOAD_SIZE; j++)
                {

                    sangoma_.data[i] = i < sangoma_.header.length ? orm_info.data[(i << comb) + i] : 0;

                }
                putData(&sangoma_, sizeof(sangoma_));
            }
        }

        DeferredWriter& SangomaMediaClient::GetWritter()
        {
            return writer_;
        }

        int SangomaMediaClient::putData(const void* data, int size)
        {
            int rc = writer_.Write(data, size);

            if (rc < 0 && rc != -EAGAIN)
            {
                DLOGE(LOG_BEGIN("putData(%x, %d): write error, rc = %d"), DLOG_POINTER(data), size, rc);
                Close();
            }
            else
            {
                if (rc == -EAGAIN)
                {
                    DLOGW(LOG_BEGIN("putData(%x, %d): would block write, rc = %d"), DLOG_POINTER(data), size, rc);
                }

                if (writer_.IsEmpty())
                {
                    select_.ClrWrite(socket_.Handle());
                }
            }

            return rc;
        }

        int SangomaMediaClient::checkConnect()
        {
            int rc = 0;

            if (timer_.IsEnable())
            {

                if (socket_.Handle() < 0)
                {
                    rc = socket_.Open(0, 0, address_, port_, 0, O_NONBLOCK);

                    if (rc > 0)
                    {
                        DLOGI(LOG_BEGIN("checkConnect(): sangoma media client %d:%d create success, sock = %d"), address_, port_, rc);
                        select_.SetRead(rc);

                    }
                    else
                    {
                        DLOGW(LOG_BEGIN("checkConnect(): sangoma media client %d:%d is not create, rc = %d"), address_, port_, rc);
                    }
                }

                timer_.Start(interval_);
            }

            return rc;
        }

        int SangomaMediaClient::checkData()
        {
            int rc = 0;

            int fd = socket_.Handle();

            if (fd >= 0)
            {

                if (select_.IsRead(fd))
                {
                    // закрытое соединение

                    Close();

                }
                else if (select_.IsWrite(fd))
                {
                    putData();
                }

            }
            return rc;
        }

    }
}
