#ifndef _MMX_TOOLS_PIPE_OUTPUT_CHANNEL_H
#define _MMX_TOOLS_PIPE_OUTPUT_CHANNEL_H

#include "tools/ichanneldispatcher.h"
#include "tools/timer.h"
#include "tools/deffwriter.h"
#include "ipc/pchannel.h"
#include "net/select_ex.h"

#include <vector>

namespace mmx
{
    namespace tools
    {
        class PipeOutputChannel : public mmx::tools::IChannelDispatcher
        {

            ipc::PipeChannel            pipe_;
            tools::DeferredWriter       writer_;

            tools::Timer                timer_;

            net::SelectExtension&       select_;


            char                        pipe_name_[256];

            int                         interval_;

        public:

            PipeOutputChannel(const char* pipe_name, mmx::net::SelectExtension& select, int interval = 2000);
            PipeOutputChannel(PipeOutputChannel&& channel);

            ~PipeOutputChannel() override;
            int Dispatch() override;
            int GetLeftTimeWork() const override;
            int Close() override;
           //void Reset() override;
            bool IsDown() const override;

            int PutData(const void* data, int size);
            void Drop();


        private:
            int checkConnect();
            int checkRead();
            int defferedWrite();
        };
    }
}

#endif
