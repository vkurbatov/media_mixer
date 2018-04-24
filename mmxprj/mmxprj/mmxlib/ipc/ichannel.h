#ifndef _MMX_IPC_I_CHANNEL_H
#define _MMX_IPC_I_CHANNEL_H

#include "io.h"

namespace mmx
{
    namespace ipc
    {

        class IChannel : public IIO
        {
        public:

            virtual ~IChannel() {};
            virtual int Open(const char*, ...) = 0;
            virtual int Close() = 0;
            virtual int Handle() const = 0;
            virtual const char* Name() const = 0;

        };
    }
}

#endif
