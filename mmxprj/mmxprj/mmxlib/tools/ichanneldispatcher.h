#ifndef _MMX_TOOLS_I_CHANNEL_DISPATCHER_H
#define _MMX_TOOLS_I_CHANNEL_DISPATCHER_H

namespace mmx
{
    namespace tools
    {
        enum dispatch_flags_t : unsigned char
        {
            DISPATCH_INIT = 1,
            DISPATCH_IO = 2,
            DISPATCH_TIMER = 4,
            DISPATCH_ERROR = 8
        };

        class IChannelDispatcher
        {

        public:
            virtual ~IChannelDispatcher() {};
            virtual int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) = 0;
            virtual int QueryOrderTimeout() const = 0;
            virtual int Close() = 0;
            //virtual void Reset() = 0;
            virtual bool IsDown() const = 0;
            virtual bool IsReadyData() const = 0;
        };
    }
}

#endif
