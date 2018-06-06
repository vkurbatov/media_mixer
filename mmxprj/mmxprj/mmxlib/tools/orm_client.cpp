#include "orm_client.h"
#include "timer.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/tcp.h>

#define RECIVE_BUFFER_SIZE  1600     //MTU + запас

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("OrmClient", msg)

namespace mmx
{
    namespace tools
    {
        OrmClient::OrmClient(net::Socket&& socket, net::SelectExtension& select) :
            socket_(std::move(socket)),
            select_(select),
            adapter_(socket_),
            writer_(adapter_),
            data_(0)
        {
            int fd = socket_.Handle();

            data_.reserve(RECIVE_BUFFER_SIZE);


            if (fd >= 0)
            {
                socket_.SetOption(SOL_TCP, TCP_NODELAY, (int)1);
                socket_.SetOption(SOL_TCP, TCP_CORK, (int)0);
                //socket_.SetOption(SOL_TCP, TCP_MAXSEG, (int)1039);
                select_.SetRead(fd);

            }
        }

        OrmClient::OrmClient(OrmClient&& channel) :
            socket_(std::move(channel.socket_)),
            select_(channel.select_),
            adapter_(socket_),
            writer_(adapter_),
            data_(std::move(channel.data_))
        {

        }

        OrmClient::~OrmClient()
        {
            Close();
        }

        int OrmClient::Dispatch(dispatch_flags_t dispatch, void* context)
        {
            int rc = 0;

            int fd = socket_.Handle();

            if (fd >= 0)
            {
                if (select_.IsRead(fd))
                {
                    //data_.clear();

                    rc = socket_.Recv(data_.data(), RECIVE_BUFFER_SIZE);

                    if (rc <= 0 && rc != -EAGAIN)
                    {
                        DLOGE(LOG_BEGIN("Dispatch(%d, %x): error read data, rc = %d"), dispatch, DLOG_POINTER(context), rc);

                        Close();
                    }
                    else
                    {
                        if (rc == -EAGAIN)
                        {
                            DLOGW(LOG_BEGIN("Dispatch(%d, %x): would block read, rc = %d"), dispatch, DLOG_POINTER(context), rc);
                            Close();

                        }
                        else
                        {
                            data_.resize(rc);
                        }
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

        int OrmClient::QueryOrderTimeout() const
        {
            return net::INFINITE_TIMEOUT;
        }

        int OrmClient::Close()
        {

            Drop();

            if (socket_.Handle() >= 0)
            {

                select_.Set(socket_.Handle());
                socket_.Close();

            }
        }

        bool OrmClient::IsDown() const
        {
            return socket_.Handle() < 0;
        }

        bool OrmClient::IsReadyData() const
        {
            return false;
        }

        const std::vector<char>& OrmClient::GetQuery() const
        {
            return data_;
        }


        int OrmClient::PutData(const mmx::headers::ORM_INFO_PACKET& orm_info)
        {
            return putData(&orm_info.header.order_header, mmx::headers::ORDER_645_2_PACKET_SIZE + 1);
        }

        void OrmClient::Drop()
        {
            data_.clear();
        }

        int OrmClient::putData(const void* data, int size)
        {
            int rc = writer_.Write(data, size);

            //int rc = socket_.Send(data, size);

            //tools::Timer::Sleep(10);

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
                    Close();
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
