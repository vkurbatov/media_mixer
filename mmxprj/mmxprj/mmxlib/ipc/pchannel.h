#ifndef _MMX_IPC_PIPE_CHANNEL_H
#define _MMX_IPC_PIPE_CHANNEL_H

#define MMX_PIPE_NAME_LEN   256

#include <memory>
#include "ichannel.h"

namespace mmx
{
    namespace ipc
    {
        class PipeChannel : public IChannel
        {
            int handle_;
            char pipename_[MMX_PIPE_NAME_LEN];
            int mode_;
            int access_;
        public:
            PipeChannel();
            //PipeChannel(const PipeChannel& channel);
            PipeChannel(PipeChannel&& channel);
            // IIO
            int Write(const char* msg, int size, int flags = 0) override;
            int Read(char* msg, int size, int flags = 0) override;
            // IChannel
            ~PipeChannel() override;
            int Open(const char* name, int mode, int access = 0) override;
            int Close() override;
            int Handle() const override;
            const char* Name() const override;
            int Mode() const override;
            int Access() const override;
        };
    }
}

#endif
