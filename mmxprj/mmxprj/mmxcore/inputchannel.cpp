#include "inputchannel.h"

namespace mmxmux
{
    InputChannel::InputChannel(mmx::net::SelectExtension& select, mmx::data::DataPacket& data_packet) :
        select_(select),
        data_packet_(data_packet)
    {

    }

    InputChannel::InputChannel(InputChannel&& channel) :
        select_(channel.select_),
        data_packet_(channel.data_packet_)
    {

    }

    InputChannel::~InputChannel()
    {
        Close();
    }

    int InputChannel::Dispatch()
    {

    }

    int InputChannel::GetLeftTimeWork() const
    {
        return pipe_.Handle() < 0 ? timer_.Left() : mmx::net::INFINITE_TIMEOUT;
    }

    int InputChannel::Close()
    {
        select_.Set(pipe_.Handle());
        return pipe_.Close();
    }
}
