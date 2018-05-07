#ifndef _MMX_TOOLS_ASYNC_ASYNC_UNIT_H
#define _MMX_TOOLS_ASYNC_ASYNC_UNIT_H

#include "iasyncunit.h"

namespace mmx
{
    namespace tools
    {
        namespace async
        {

            class AsyncUnit : public IAsyncUnit
            {


            public:

                // IAsyncUnit
                bool IsCanRead() const override;
                bool IsCanWrite() const override;
                int IODevice(aio_info_t &rdata, aio_info_t &wdata, int &event_mask) override;
                int OpenDevice() override;
                int CloseDevice() override;
                int Handle() const override;

            protected:

                virtual int openDevice() = 0;
                virtual int closeDevice() = 0;
                virtual int read(void* data, int size) = 0;
                virtual int write(void* data, int size) = 0;
            };
        }
    }
}

#endif
