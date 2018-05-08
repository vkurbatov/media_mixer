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
                Timer                   timer_;

            public:
                AsyncUnit(ipc::IChannel& channel, ipc::IIO& io);
                AsyncUnit(AsyncUnit&& unit);
                ~AsyncUnit();
                // IAsyncUnit interface
                async_state_mask_t GetState() const override;
                ipc::IChannel &GetChannel() override;
                ipc::IIO &GetIO() override;
                int QueryWrite(aio_info_t& aio_info) override;
                int OnRead(const aio_info_t& aio_info) override;
                int QueryTimerWork() const override;
            protected:
                virtual int queryWrite(aio_info_t& aio_info) = 0;
                virtual int onRead(const aio_info_t& aio_info) = 0;

                virtual bool canOpen() const;
                virtual bool canClose() const;
                virtual bool canRead() const;
                virtual bool canWrite() const;
                virtual bool canTimer() const;
                virtual bool canRemove() const;

            };
        }
    }
}

#endif
