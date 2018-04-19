#include "orm_client.h"

#include <errno.h>

#define RECIVE_BUFFER_SIZE  1600     //MTU + запас

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
                        Close();
                    }
                    else
                    {
                        data_.resize(rc);
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
            return putData(&orm_info.header.order_header, mmx::headers::ORDER_645_2_PACKET_SIZE);
        }

        void OrmClient::Drop()
        {
            data_.clear();
        }

        int OrmClient::putData(const void* data, int size)
        {
            int rc = writer_.Write(data, size);

            if (rc < 0 && rc != -EAGAIN)
            {
                Close();
            }
            else
            {
                if (writer_.IsEmpty())
                {
                    select_.ClrWrite(socket_.Handle());
                }
            }

            return rc;
        }

    }
}
