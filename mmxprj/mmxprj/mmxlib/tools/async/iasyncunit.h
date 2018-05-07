#ifndef _MMX_TOOLS_ASYNC_I_ASYNC_UNIT_H
#define _MMX_TOOLS_ASYNC_I_ASYNC_UNIT_H

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

            class IAsyncUnit
            {

            public:
                virtual ~IAsyncUnit() {};
                virtual bool IsCanRead() const = 0;
                virtual bool IsCanWrite() const = 0;
                virtual int IODevice(aio_info_t& rdata, aio_info_t& wdata, int& event_mask) = 0;
                virtual int OpenDevice() = 0;
                virtual int CloseDevice() = 0;
                virtual int Handle() const = 0;

            };
        }
    }
}

#endif
