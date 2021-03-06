#include "sangoma_media_sender.h"

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
        SangomaMediaSender::SangomaMediaSender(net::address_t address, net::port_t port, net::SelectExtension& select, int interval) :
            address_(address),
            port_(port),
            socket_(SOCK_DGRAM, IPPROTO_UDP),
            select_(select),
            adapter_(socket_),
            writer_(adapter_),
            data_(sizeof(headers::SANGOMA_PACKET)),
            interval_(interval)
        {

            timer_.Start(0);
            std::memset(&sangoma_, 0, sizeof(sangoma_));

        }

        SangomaMediaSender::SangomaMediaSender(SangomaMediaSender&& channel) :
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

        SangomaMediaSender::~SangomaMediaSender()
        {
            Close();
        }

        int SangomaMediaSender::Dispatch(dispatch_flags_t dispatch, void* context)
        {
            int rc = 0;

            checkConnect();
            checkData();

            return rc;
        }

        int SangomaMediaSender::QueryOrderTimeout() const
        {
            return socket_.Handle() < 0 && timer_.IsStarted()
                    ? timer_.Left()
                    : mmx::net::INFINITE_TIMEOUT;
        }

        int SangomaMediaSender::Close()
        {
            writer_.Drop();

            if (socket_.Handle() >= 0)
            {

                select_.Set(socket_.Handle());
                socket_.Close();

            }
        }

        bool SangomaMediaSender::IsDown() const
        {
            return socket_.Handle() < 0;
        }

        bool SangomaMediaSender::IsReadyData() const
        {
            return false;
        }

        int SangomaMediaSender::PutMedia(const void* data, int size, unsigned char mcl_a, unsigned char mcl_b)
        {

            int comb = (int)(mcl_b == 0xFF) || (mcl_a == mcl_b);

            mcl_a += (unsigned char)((mcl_a != 0xFF) && ((mcl_a & 0x1F) > 15));
            mcl_b += (unsigned char)((mcl_b != 0xFF) && ((mcl_b & 0x1F) > 15));

            return comb != 0 ?
                      combineSend(data, size, mcl_a) :
                      separatedSend(data, size, mcl_a, mcl_b);
        }

        int SangomaMediaSender::combineSend(const void* data, int size, unsigned char mcl_a)
        {
            int rc = -EINVAL;

            if (data != nullptr && size > 0)
            {
                while (size > 0)
                {
                    int seg_size = size > mmx::headers::SI_MAX_PYLOAD_SIZE ? mmx::headers::SI_MAX_PYLOAD_SIZE : size;

                    sangoma_.header.packet_id++;
                    sangoma_.header.lid = mcl_a;
                    sangoma_.header.pid = 1;
                    sangoma_.header.length = seg_size + sizeof(sangoma_.header);

                    std::memcpy(sangoma_.data, data, seg_size);

                    if (seg_size < mmx::headers::SI_MAX_PYLOAD_SIZE)
                    {
                        std::memset(sangoma_.data  + seg_size, mmx::headers::SI_MAX_PYLOAD_SIZE - seg_size, 0);
                    }

                    putData(&sangoma_, sizeof(sangoma_));

                    size -= seg_size;
                    data = ((const char*)data) + seg_size;

                    // Hardcode
                    //tools::Timer::Sleep(64);
                }
            }

            return rc;
        }

        int SangomaMediaSender::separatedSend(const void* data, int size, unsigned char mcl_a, unsigned char mcl_b)
        {
            int rc = -EINVAL;

            if (data != nullptr && size > 0)
            {
                size /= 2;

                while (size > 0)
                {
                    int seg_size = size > mmx::headers::SI_MAX_PYLOAD_SIZE ? mmx::headers::SI_MAX_PYLOAD_SIZE : size;

                    for (int i = 0; i < 2; i++)
                    {

                        sangoma_.header.packet_id++;
                        sangoma_.header.lid = i == 0 ? mcl_a : mcl_b;
                        sangoma_.header.pid = 1;
                        sangoma_.header.length = seg_size + sizeof(sangoma_.header);


                        for (int j = 0; j < seg_size; j++)
                        {
                            sangoma_.data[j] = ((const char*)data)[j * 2 + i];
                        }

                        if (seg_size < mmx::headers::SI_MAX_PYLOAD_SIZE)
                        {
                            std::memset(sangoma_.data + seg_size, mmx::headers::SI_MAX_PYLOAD_SIZE - seg_size, 0);
                        }

                        putData(&sangoma_, sizeof(sangoma_));
                    }

                    size -= seg_size;
                    data = ((const char*)(data)) + seg_size;
                }
            }

            return rc;
        }

        DeferredWriter& SangomaMediaSender::GetWritter()
        {
            return writer_;
        }

        int SangomaMediaSender::putData(const void* data, int size)
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
                else
                {
                    DLOGD(LOG_BEGIN("putData(%x, %d): data write complete %d bytes"), DLOG_POINTER(data), size, rc);
                }

                if (writer_.IsEmpty())
                {
                    select_.ClrWrite(socket_.Handle());
                }
            }

            return rc;
        }

        int SangomaMediaSender::checkConnect()
        {
            int rc = 0;

            if (timer_.IsEnable())
            {

                if (socket_.Handle() < 0)
                {
                    rc = socket_.Open(0, 0, address_, port_, 0, O_NONBLOCK);

                    if (rc > 0)
                    {
                        DLOGI(LOG_BEGIN("checkConnect(): sangoma media client " DLOG_IP_FMT ":%d create success, sock = %d"), DLOG_IP(address_), port_, rc);
                        select_.SetRead(rc);

                    }
                    else
                    {
                        DLOGW(LOG_BEGIN("checkConnect(): sangoma media client " DLOG_IP_FMT ":%d is not create, rc = %d"), DLOG_IP(address_), port_, rc);
                    }
                }

                timer_.Start(interval_);
            }

            return rc;
        }

        int SangomaMediaSender::checkData()
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
