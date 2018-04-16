#ifndef _MMX_TOOLS_I_CHANNEL_DISPATCHER_H
#define _MMX_TOOLS_I_CHANNEL_DISPATCHER_H

namespace mmx
{
    namespace tools
    {
        class IChannelDispatcher
        {

        public:
            virtual ~IChannelDispatcher() {};
            virtual int Dispatch() = 0;
            virtual int GetLeftTimeWork() const = 0;
            virtual int Close() = 0;
            //virtual void Reset() = 0;
            virtual bool IsDown() const = 0;
        };
    }
}

#endif
