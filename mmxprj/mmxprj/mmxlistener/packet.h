#ifndef _MMX_PACKET_H
#define _MMX_PACKET_H

#include <vector>

namespace mmxlst
{
    using packet_data_t = char;

    class Packet
    {
        std::vector<packet_data_t> data_;
    public:
        Packet();
        Packet(const packet_data_t* data, int size);
        Packet(const Packet& packet);
        Packet(Packet&& packet);
        Packet& operator=(const Packet& packet);
        Packet& operator=(Packet&& packet);
        operator packet_data_t* ();
        packet_data_t* Data();
        int Size() const;
    };
}

#endif
