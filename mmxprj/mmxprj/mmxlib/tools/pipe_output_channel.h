#ifndef _MMX_TOOLS_PIPE_OUTPUT_CHANNEL_H
#define _MMX_TOOLS_PIPE_OUTPUT_CHANNEL_H

#include "tools/ichanneldispatcher.h"
#include "tools/timer.h"
#include "tools/deffwriter.h"
#include "ipc/pchannel.h"
#include "net/select_ex.h"

#include "data/dpwriter.h"

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

            std::vector<char>           data_;
            data::DataPacketWriter      dp_writter_;


            char                        pipe_name_[MMX_PIPE_NAME_LEN];

            int                         interval_;

            unsigned char               channel_;

            unsigned short              pack_id_;


        public:

            PipeOutputChannel(const char* pipe_name_prefix, unsigned char channel, mmx::net::SelectExtension& select, int interval = 2000);
            PipeOutputChannel(PipeOutputChannel&& channel);

            ~PipeOutputChannel() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int QueryOrderTimeout() const override;
            int Close() override;
           //void Reset() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            data::DataPacketWriter& GetWritter();
            unsigned char GetChannelId() const;

            int Send();
            void Drop();


        private:
            int checkConnect();
            int checkRead();
            int defferedWrite();
        };
    }
}

#endif
