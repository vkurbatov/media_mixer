#ifndef _MMX_TOOLS_ASYNC_ASYNC_UNIT_H
#define _MMX_TOOLS_ASYNC_ASYNC_UNIT_H

#include "iasyncunit.h"
#include "../timer.h"

namespace mmx
{
    namespace tools
    {
        namespace async
        {

            class AsyncUnit : public IAsyncUnit
            {
                ipc::IChannel&          channel_;
                ipc::IIO&               io_;
                Timer                   work_timer_;
                Timer                   conn_timer_;
                bool                    removable_;



            public:
                AsyncUnit(ipc::IChannel& channel, ipc::IIO& io, int conn_time = 0, int work_time = 0, bool removable = false);
                AsyncUnit(AsyncUnit&& unit);
                ~AsyncUnit();

                // IAsyncUnit interface
                int QueryWrite(aio_info_t& aio_info) override;
                int OnRead(const aio_info_t& aio_info) override;
                int QueryTimerWork() const override;

                // IChannel interface
                int Open() override;
                int Close() override;
                int Handle() const override;

                // IIO interface
                int Write(const void *, int, int) override;
                int Read(void *, int, int) override;
                bool IsCanWrite() const override;
                bool IsCanRead() const override;

            protected:
                virtual int queryWrite(aio_info_t& aio_info) = 0;
                virtual int onRead(const aio_info_t& aio_info) = 0;

                // IAsyncUnit interface
            public:
                bool IsCanOpen() const override;
                bool IsCanRemove() const override;
            };
        }
    }
}

#endif
