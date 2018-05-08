#include "asyncunit.h"

namespace mmx
{
    namespace tools
    {
        namespace async
        {

            AsyncUnit::AsyncUnit(ipc::IChannel &channel, ipc::IIO &io) :
                channel_(channel),
                io_(io)
            {

            }

            AsyncUnit::AsyncUnit(AsyncUnit &&unit) :
                channel_(unit.channel_),
                io_(unit.io_)
            {

            }

            AsyncUnit::~AsyncUnit()
            {

            }

            async_state_mask_t AsyncUnit::GetState() const
            {
                return AU_CAN_CLOSE     * (int)canClose()   |
                       AU_CAN_OPEN      * (int)canOpen()    |
                       AU_CAN_READ      * (int)canRead()    |
                       AU_CAN_WRITE     * (int)canWrite()   |
                       AU_CAN_TIMER     * (int)canTimer()   |
                       AU_CAN_REMOVE    * (int)canRemove();


            }

            ipc::IChannel &AsyncUnit::GetChannel()
            {
                return channel_;
            }

            ipc::IIO &AsyncUnit::GetIO()
            {
                return io_;
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
                return timer_.IsStarted() ? timer_.Left() : -1;
            }

            bool AsyncUnit::canOpen() const
            {
                return channel_.Handle() < 0 && timer_.IsEnable();
            }

            bool AsyncUnit::canClose() const
            {
                return channel_.Handle() >= 0;
            }

            bool AsyncUnit::canRead() const
            {
                return io_.IsCanRead();
            }

            bool AsyncUnit::canWrite() const
            {
                return io_.IsCanWrite();
            }

            bool AsyncUnit::canTimer() const
            {
                return channel_.Handle() < 0;
            }

            bool AsyncUnit::canRemove() const
            {
                return false;
            }

        }
    }
}
