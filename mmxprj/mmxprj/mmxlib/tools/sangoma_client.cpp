#include "sangoma_client.h"

#include <errno.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("SangomaClient", msg)

namespace mmx
{
    namespace tools
    {
        SangomaClient::SangomaClient(net::Socket&& socket, net::SelectExtension& select) :
            socket_(std::move(socket)),
            select_(select),
            adapter_(socket_),
            writer_(adapter_),
            data_(sizeof(headers::SANGOMA_PACKET)),
            query_(nullptr)
        {
            int fd = socket_.Handle();

            if (fd >= 0)
            {
                select_.SetRead(fd);
            }
        }

        SangomaClient::SangomaClient(SangomaClient&& channel) :
            socket_(std::move(channel.socket_)),
            select_(channel.select_),
            adapter_(socket_),
            writer_(adapter_),
            data_(std::move(channel.data_)),
            query_(channel.query_)
        {
            channel.query_ = nullptr;
        }

        SangomaClient::~SangomaClient()
        {
            Close();
        }

        int SangomaClient::Dispatch(dispatch_flags_t dispatch, void* context)
        {
            int rc = 0;

            int fd = socket_.Handle();

            if (fd >= 0)
            {
                if (select_.IsRead(fd))
                {
                    rc = socket_.Recv(data_.data(), data_.size());

                    if (rc <= 0 && rc != -EAGAIN)
                    {
                        DLOGE(LOG_BEGIN("Dispatch(%d, %x): error read data, rc = %d"), dispatch, DLOG_POINTER(context), rc);

                        Close();
                    }

                    if (rc >= sizeof(headers::SANGOMA_HEADER))
                    {
                        query_ = (const headers::SANGOMA_PACKET*)data_.data();

                        if (query_->header.length > rc)
                        {
                            query_ = nullptr;
                        }

                    }
                    else
                    {
                        DLOGE(LOG_BEGIN("Dispatch(%d, %x): too few recievet bytes, rc = %d"), dispatch, DLOG_POINTER(context), rc);
                    }
                }
                if (select_.IsWrite(fd))
                {
                    if (!writer_.IsEmpty())
                    {

                        rc = putData();

                    }


                }
            }

            return rc;
        }

        int SangomaClient::QueryOrderTimeout() const
        {
            return net::INFINITE_TIMEOUT;
        }

        int SangomaClient::Close()
        {

            Drop();

            if (socket_.Handle() >= 0)
            {

                select_.Set(socket_.Handle());
                socket_.Close();

            }
        }

        bool SangomaClient::IsDown() const
        {
            return socket_.Handle() < 0;
        }

        bool SangomaClient::IsReadyData() const
        {
            return query_ != nullptr;
        }


        const headers::SANGOMA_PACKET* SangomaClient::GetQuery()
        {
            return query_;
        }

        int SangomaClient::PutAnswer(const headers::SANGOMA_PACKET& answer)
        {
            return putData(&answer, sizeof(answer));
        }

        void SangomaClient::Drop()
        {
            query_ = nullptr;
        }

        int SangomaClient::putData(const void* data, int size)
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

    }
}
