#ifndef _MMXMUX_INPUT_CHANNEL_H
#define _MMXMUX_INPUT_CHANNEL_H

#include "mmxlib/tools/ichanneldispatcher.h"
#include "mmxlib/ipc/pchannel.h"
#include "mmxlib/net/select_ex.h"
#include "mmxlib/tools/timer.h"
#include "mmxlib/data/datapacket.h"

namespace mmxmux
{
    class InputChannel : public mmx::tools::IChannelDispatcher
    {

        mmx::ipc::PipeChannel           pipe_;
        mmx::tools::Timer               timer_;

        mmx::net::SelectExtension&      select_;
        mmx::data::DataPacket&          data_packet_;

    public:
        InputChannel(mmx::net::SelectExtension& select, mmx::data::DataPacket& data_packet);
        InputChannel(InputChannel&& channel);

        ~InputChannel() override;
        int Dispatch() override;
        int GetLeftTimeWork() const override;
        int Close() override;
    };
}

#endif
