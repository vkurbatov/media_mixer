#ifndef _MMX_SNIFFER_IP_PACKET_POOL_H
#define _MMX_SNIFFER_IP_PACKET_POOL_H

#include "ippacket.h"

#include <unordered_map>
#include <queue>
#include <vector>
#include <typeinfo>

namespace mmx
{
    namespace sniffers
    {

        /*static const int DEFAULT_PACKET_POOL_FREE_QUEUE_MAX_SIZE = 10;
        static const int DEFAULT_GARBAGE_MAX_POOL_SIZE = 10;
        static const int DEFAULT_GARBAGE_TIME_LIFE = 100;*/

        class IPPacketPool
        {
            std::unordered_map<std::uint64_t, IPPacket> pool_;
            std::queue<IPPacket> q_free_;
            std::vector<IPPacket*> rm_list_;

            int max_free_queue_size_;
            int min_garbage_size_;
            int garbage_time_life_;

        public:
            IPPacketPool(int max_free_queue_size = -1,
                         int min_garbage_size = 0,
                         int garbage_time_life = 0);
            ~IPPacketPool();

            IPPacket* GetPacket(unsigned int src_address, unsigned short pack_id);
            bool Release(IPPacket* packet);
            int ClearGarbage(int time_life = 0);
            int Count() const;
            void Reset();
        private:
            std::uint64_t getKey(unsigned int src_address, unsigned short pack_id);
        };
    }
}

#endif
