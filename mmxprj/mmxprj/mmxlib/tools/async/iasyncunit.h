#ifndef _MMX_TOOLS_ASYNC_I_ASYNC_UNIT_H
#define _MMX_TOOLS_ASYNC_I_ASYNC_UNIT_H

#include "ipc/ichannel.h"
#include "ipc/io.h"

namespace mmx
{
    namespace tools
    {
        namespace async
        {

            typedef struct
            {
                void*   data;
                int     size;
            }aio_info_t;

            class IAsyncUnit : public ipc::IChannel, public ipc::IIO
            {

            public:
                virtual ~IAsyncUnit() {};

                virtual int QueryWrite(aio_info_t& aio_info) = 0;
                virtual int OnRead(const aio_info_t& aio_info) = 0;
                virtual int QueryTimerWork() const = 0;

                virtual bool IsCanOpen() const = 0;
                virtual bool IsCanRemove() const = 0;

            };
        }
    }
}

#endif
