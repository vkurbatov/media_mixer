#ifndef _MMX_IPC_PIPE_CHANNEL_H
#define _MMX_IPC_PIPE_CHANNEL_H

#define MMX_PIPE_NAME_LEN   256

#include <memory>
#include "ichannel.h"
#include "io.h"

namespace mmx
{
    namespace ipc
    {
        class PipeChannel : public IChannel, public IIO
        {
            int handle_;
            char pipename_[MMX_PIPE_NAME_LEN];
            int mode_;
            int access_;

        public:
            PipeChannel(const char *pipe_name, int mode = 0, int access = 0);
            //PipeChannel(const PipeChannel& channel);
            PipeChannel(PipeChannel&& channel);
            const char* Name() const;

            // IIO
            int Write(const void* msg, int size, int flags = 0) override;
            int Read(void* msg, int size, int flags = 0) override;
            bool IsCanWrite() const override;
            bool IsCanRead() const override;

            // IChannel
            ~PipeChannel() override;
            int Open() override;
            int Close() override;
            int Handle() const override;

        };
    }
}

#endif
