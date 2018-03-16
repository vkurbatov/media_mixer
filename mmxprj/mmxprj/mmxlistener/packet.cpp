#include <memory>
#include "packet.h"

namespace mmxlst
{
    Packet::Packet() :
        data_()
    {

    }
    Packet::Packet(const packet_data_t* data, int size) :
        data_(data, data + size)
    {

    }

    Packet::Packet(const Packet& packet) :
        data_(packet.data_)
    {

    }

    Packet::Packet(Packet&& packet) :
        data_(std::move(packet.data_))
    {

    }

    Packet& Packet::operator=(const Packet& packet)
    {
        data_ = packet.data_;
        return *this;
    }

    Packet& Packet::operator=(Packet&& packet)
    {
        data_ = std::move(packet.data_);
        return *this;
    }

    Packet::operator packet_data_t* ()
    {
        return Data();
    }

    packet_data_t* Packet::Data()
    {
        return data_.data();
    }

    int Packet::Size() const
    {
        return data_.size();
    }

}
