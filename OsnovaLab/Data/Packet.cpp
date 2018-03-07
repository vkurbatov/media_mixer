#include <cstdlib>
#include "OsnovaLab/Data/Packet.h"

namespace OsnovaLab
{
	namespace Data
	{
        Packet::Packet() :
            data_(nullptr),
            size_(0)
        {

        }

        Packet::Packet(const raw_data_ptr_t data, const collection_size_t size) :
            data_(size > 0 ? new packet_data_t[size] : nullptr),
            size_(size > 0 ? size : 0)
        {
            if (data_ != nullptr)
            {
                std::copy((char*)data, ((char*)data) + size, data_.get());
            }
        }

        Packet::Packet(const Packet& packet) :
            Packet(packet.data_.get(), packet.size_)
        {

        }


        Packet::Packet(Packet&& packet) :
            data_(std::move(packet.data_)),
            size_(packet.size_)
        {
            packet.size_ = 0;
        }

        Packet::~Packet()
        {

        }

        Packet& Packet::operator=(const Packet& packet)
        {
            data_.reset((size_ = packet.size_) > 0 ?
                            new packet_data_t[packet.size_] :
                            nullptr);
        }

        Packet& Packet::operator=(Packet&& packet)
        {
            data_ = std::move(packet.data_);
            size_ = packet.size_;
            packet.size_ = 0;
        }

        Packet::operator const packet_data_ptr_t() const
        {
            return data_.get();
        }

        Packet::operator const raw_data_ptr_t() const
        {
            return static_cast<raw_data_ptr_t>(data_.get());
        }

        const packet_data_ptr_t Packet::Data() const
        {
            return data_.get();
        }

        const collection_size_t Packet::Size() const
        {
            return size_;
        }

        Packet* Packet::Clone()
        {
            return new Packet(*this);
        }
	}
}


