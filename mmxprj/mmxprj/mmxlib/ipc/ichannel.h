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
            virtual int Open(const char*, int, int) = 0;
            virtual int Close() = 0;
//            virtual int Write(const char*, int, int = 0) = 0;
 //           virtual int Read(char*, int, int = 0) = 0;
            virtual int Handle() const = 0;
            virtual const char* Name() const = 0;
            virtual int Mode() const = 0;
            virtual int Access() const = 0;

        };
    }
}

#endif
