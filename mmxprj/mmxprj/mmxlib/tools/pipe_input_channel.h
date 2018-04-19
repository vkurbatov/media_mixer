#ifndef _MMX_TOOLS_PIPE_INPUT_CHANNEL_H
#define _MMX_TOOLS_PIPE_INPUT_CHANNEL_H

#include "tools/ichanneldispatcher.h"
#include "tools/timer.h"
#include "ipc/pchannel.h"
#include "net/select_ex.h"

#include <vector>

namespace mmx
{
    namespace tools
    {
        class PipeInputChannel : public mmx::tools::IChannelDispatcher
        {

            ipc::PipeChannel            pipe_;
            tools::Timer                timer_;
            std::vector<char>           data_;

            net::SelectExtension&       select_;


            char                        pipe_name_[MMX_PIPE_NAME_LEN];

            int                         read_bytes_;
            int                         interval_;
            unsigned char               channel_;

        public:

            PipeInputChannel(const char* pipe_name_prefix, unsigned char channel, mmx::net::SelectExtension& select, int interval = 2000);
            PipeInputChannel(PipeInputChannel&& channel);

            ~PipeInputChannel() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int QueryOrderTimeout() const override;
            int Close() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            const void* Data() const;
            int Size() const;
            void Drop();


        private:
            int checkConnect();
            int readData();
        };
    }
}

#endif
