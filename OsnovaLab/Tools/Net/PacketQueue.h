#ifndef _OL_TOOLS_NET_PACKET_QUEUE_H
#define _OL_TOOLS_NET_PACKET_QUEUE_H

#include "OsnovaLab/Data/Queue.h"
#include "OsnovaLab/Data/Packet.h"

namespace OsnovaLab
{
    namespace Tools
    {
        namespace Net
        {
            using packet_t = Data::Packet;

            class PacketQueue
            {


                Data::Queue<Data::Packet> queue_;
                Data::collection_size_t total_size_;
                //std::queue<packet_t> queue_;

            public:
                PacketQueue();
                ~PacketQueue();

                Data::collection_size_t Count() const;
                Data::collection_size_t TotalSize() const;
                Data::collection_size_t Push(Data::packet_data_ptr_t data, Data::collection_size_t size, bool append = false);
                Data::collection_size_t Pop(Data::packet_data_ptr_t data, Data::collection_size_t size, bool read_all = false);
                packet_t* Front();

            };
        }
    }
}

#endif
