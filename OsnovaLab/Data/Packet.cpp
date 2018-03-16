#include <cstdlib>
#include "OsnovaLab/Data/Packet.h"

namespace OsnovaLab
{
	namespace Data
	{
        Packet::Packet() :
            data_(),
            //size_(0),
            offset_(0)
        {

        }

        Packet::Packet(const packet_data_ptr_t data, const collection_size_t size) :
            data_(data, data + size),
            //size_(size > 0 ? size : 0),
            offset_(0)
        {

        }

        Packet::Packet(const Packet& packet) :
            data_(packet.data_),
            offset_(packet.offset_)
        {

        }

        Packet::Packet(Packet&& packet) :
            data_(std::move(packet.data_)),
            offset_(packet.offset_)
        {
            packet.offset_ = 0;
        }

        Packet::~Packet()
        {

        }
/*
        packet_data_ptr_t Packet::packet_alloc(const packet_data_ptr_t pattern, const collection_size_t size)
        {
            packet_data_ptr_t rc = nullptr;

            if (size > 0)
            {
                std::unique_ptr<packet_data_t> ptr(new packet_data_t[size]);
                if (pattern != nullptr)
                {
                    std::copy(pattern, pattern + size, ptr.get());
                }
                rc = ptr.release();
            }

            return rc;
        }
*/
        Packet& Packet::operator=(const Packet& packet)
        {
            //data_.reset(packet_alloc(packet.data_.get(), size_ = packet.size_));
            data_ = packet.data_;
            offset_ = packet.offset_;
        }

        Packet& Packet::operator=(Packet&& packet)
        {
            data_ = std::move(packet.data_);
            offset_ = packet.offset_;
            packet.offset_ = 0;
        }

        collection_size_t Packet::Pop(packet_data_ptr_t data, collection_size_t size)
        {
            if (size > Size())
                size = Size();

            if (size > 0)
            {
                if (data != nullptr)
                {
                    //data_.
                    std::copy((packet_data_ptr_t)data_.data() + offset_, (packet_data_ptr_t)data_.data() + offset_ + size, (packet_data_ptr_t)data);
                }
                offset_ += size;
            }
            return size;
        }

        collection_size_t Packet::Push(const packet_data_ptr_t data, collection_size_t size)
        {
            collection_size_t rc = -1;

            if (size > 0)
            {
                auto off_bf = offset_;
                std::copy(data, data + size, std::back_inserter(data_));
                rc = size;
                auto off_af = offset_;
                off_bf = off_af;
            }

            return rc;
        }

        void Packet::Reset()
        {
            offset_ = 0;
        }

        Packet::operator const_packet_data_ptr_t() const
        {
            return Data();
        }


        const_packet_data_ptr_t Packet::Data() const
        {
            return data_.data() + offset_;
        }

        const collection_size_t Packet::Size() const
        {
            return (collection_size_t)data_.size() - offset_;
        }

        std::shared_ptr<Packet> Packet::Clone()
        {
            return std::make_shared<Packet>(*this);
        }
	}
}


