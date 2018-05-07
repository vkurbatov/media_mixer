#ifndef _MMX_IPC_I_CHANNEL_H
#define _MMX_IPC_I_CHANNEL_H

namespace mmx
{
    namespace ipc
    {

        class IChannel
        {
        public:

            virtual ~IChannel() {};
            virtual int Config(int argn, ...) = 0;
            virtual int Open() = 0;
            virtual int Close() = 0;
            virtual int Handle() const = 0;
        };
    }
}

#endif
