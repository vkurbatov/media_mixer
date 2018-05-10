#include "asyncunit.h"

#include <memory>

namespace mmx
{
    namespace tools
    {
        namespace async
        {

            AsyncUnit::AsyncUnit(ipc::IChannel &channel, ipc::IIO &io, int conn_time, int work_time, bool removable) :
                channel_(channel),
                io_(io),
                work_timer_(work_time, true),
                conn_timer_(conn_time, true),
                removable_(removable)
            {

            }

            AsyncUnit::AsyncUnit(AsyncUnit &&unit) :
                channel_(unit.channel_),
                io_(unit.io_),
                work_timer_(std::move(unit.work_timer_)),
                conn_timer_(std::move(unit.conn_timer_)),
                removable_(unit.removable_)
            {

            }

            AsyncUnit::~AsyncUnit()
            {

            }


            int AsyncUnit::QueryWrite(aio_info_t &aio_info)
            {
                queryWrite(aio_info);
            }

            int AsyncUnit::OnRead(const aio_info_t &aio_info)
            {
                return onRead(aio_info);
            }

            int AsyncUnit::QueryTimerWork() const
            {                
                int to = -1;



            }

            int AsyncUnit::Open()
            {
                return channel_.Open();
            }

            int AsyncUnit::Close()
            {
                return channel_.Close();
            }

            int AsyncUnit::Handle() const
            {
                return channel_.Handle();
            }

            bool mmx::tools::async::AsyncUnit::IsCanOpen() const
            {
                return channel_.Handle() < 0 && conn_timer_.IsEnable();
            }

            bool mmx::tools::async::AsyncUnit::IsCanRemove() const
            {
                return false;
            }

            int AsyncUnit::AsyncUnit::Write(const void* data, int size, int flags)
            {
                return io_.Write(data, size, flags);
            }

            int AsyncUnit::AsyncUnit::Read(void* data, int size, int flags)
            {
                return io_.Read(data, size, flags);
            }

            bool AsyncUnit::AsyncUnit::IsCanWrite() const
            {
                return io_.IsCanWrite();
            }

            bool AsyncUnit::AsyncUnit::IsCanRead() const
            {
                return io_.IsCanRead();
            }

        }
    }
}

