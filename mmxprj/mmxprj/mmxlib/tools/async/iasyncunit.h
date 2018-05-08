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


            typedef int async_state_mask_t;

            static const async_state_mask_t AU_NONE         = 0;
            static const async_state_mask_t AU_CAN_READ     = 1;
            static const async_state_mask_t AU_CAN_WRITE    = 1 << 1;
            static const async_state_mask_t AU_CAN_OPEN     = 1 << 2;
            static const async_state_mask_t AU_CAN_CLOSE    = 1 << 3;
            static const async_state_mask_t AU_CAN_TIMER    = 1 << 4;
            static const async_state_mask_t AU_CAN_REMOVE   = 1 << 5;

            typedef struct
            {
                void*   data;
                int     size;
            }aio_info_t;

            class IAsyncUnit
            {

            public:
                virtual ~IAsyncUnit() {};
                virtual async_state_mask_t GetState() const = 0;
                virtual ipc::IChannel& GetChannel() = 0;
                virtual ipc::IIO& GetIO() = 0;
                virtual int QueryWrite(aio_info_t& aio_info) = 0;
                virtual int OnRead(const aio_info_t& aio_info) = 0;
                virtual int QueryTimerWork() const = 0;

            };
        }
    }
}

#endif
