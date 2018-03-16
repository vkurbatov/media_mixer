#ifndef _MMXLST_CHANNEL_H
#define _MMXLST_CHANNEL_H

#include "packet.h"

#include <queue>


namespace mmxlst
{

    using packet_data_t = char;

    class Channel
    {
        //static const int DEFAULT_BUFFER_SIZE = 1600; // MTU + запас

        int static s_channel_id_;
        int channel_id_;
        int parent_id_;
        unsigned short port_;
        int total_size_;

        std::queue<Packet > queue_;

        //packet_data_t buff_[DEFAULT_BUFFER_SIZE];

    public:
        Channel();
        Channel(unsigned short port, int parent_id);
        Channel(Channel&& channel);
        ~Channel();
        //int Read(int sock);
        int Push(Packet&& packet);
        int PacketCount() const;
        int TotalSize() const;
        int Pop(Packet&& packet);
        bool IsEmpty() const;
        unsigned short Port() const;
        int Id() const;
        int ParentId() const;
    };
}

#endif
