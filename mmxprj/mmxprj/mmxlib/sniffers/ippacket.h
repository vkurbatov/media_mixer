#ifndef _MMX_SNIFFER_IP_PACKET_H
#define _MMX_SNIFFER_IP_PACKET_H

#include "bitset.h"
#include "headers/ip.h"
#include "istream.h"

#include "iippacket.h"

#include <chrono>

namespace mmx
{
    namespace sniffers
    {
        static const int DEFAULT_MTU_SIZE = 1600; // возьмем максимум
        static const int MAX_FRAGMEN_VALUE = 1 << 13;

        class IPPacketPool;

        class IPPacket : public IStream, public IIPPacket
        {            
            std::vector<char>   data_;

            BitSet              frag_set_;

            int needs_bytes_;
            int saved_bytes_;
            int frame_bytes_;
            bool last_packet_;
            unsigned int src_addr_;
            int pack_id_;
            int offset_;

            headers::IP4HEADER header_;

            std::chrono::time_point<std::chrono::system_clock> t_stamp_;


            friend class IPPacketPool;


        public:
            IPPacket();
            IPPacket(IPPacket&& packet);

            IPPacket& operator=(IPPacket&& packet);

            bool operator==(const IPPacket& packet);

            static int FindStart(const void* stream, int size);
            static int CheckHeader(const void* stream, int size);
            static int CheckPacket(const void* stream, int size);
            static unsigned short CheckSumm(const void* header, int size);

            // IStream
            int PutStream(const void* stream, int size, void* context) override;
            int Drop() override;
            int Reset() override;
            bool IsComplete() const override;
            bool IsBad() const override;

            //IIPPacket
            const headers::IP4HEADER* Header() const override;
            const void* Pyload() const override;
            int Size() const override;

        private:
            int putData(const void* stream, int size, int offset);


        };
    }
}

#endif
