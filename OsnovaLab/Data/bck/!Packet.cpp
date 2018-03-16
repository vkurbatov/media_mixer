#include <cstdlib>
#include "OsnovaLab/Data/Packet.h"

namespace OsnovaLab
{
	namespace Data
	{
        Packet::Packet() :
            data_(nullptr),
            size_(0),
            offset_(0)
        {

        }

        Packet::Packet(const packet_data_ptr_t data, const collection_size_t size) :
            data_(packet_alloc(data, size)),
            size_(size > 0 ? size : 0),
            offset_(0)
        {
            if (data_ != nullptr)
            {
                std::copy((char*)data, ((char*)data) + size, data_.get());
            }
        }

        Packet::Packet(const Packet& packet) :
            Packet(packet.data_.get(), packet.size_)
        {
            offset_ = packet.offset_;
        }


        Packet::Packet(Packet&& packet) :
            data_(std::move(packet.data_)),
            size_(packet.size_),
            offset_(packet.offset_)
        {
            packet.size_ = 0;
            packet.offset_ = 0;
        }

        Packet::~Packet()
        {

        }

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

        Packet& Packet::operator=(const Packet& packet)
        {
            data_.reset(packet_alloc(packet.data_.get(), size_ = packet.size_));
            offset_ = packet.offset_;
        }

        Packet& Packet::operator=(Packet&& packet)
        {
            data_ = std::move(packet.data_);
            size_ = packet.size_;
            offset_ = packet.offset_;
            packet.size_ = 0;
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
                    std::copy((packet_data_ptr_t)data_.get() + offset_, (packet_data_ptr_t)data_.get() + offset_ + size, (packet_data_ptr_t)data);
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
                // пямять еще не аллоцирована
                if (data_ == nullptr)
                {
                    data_.reset(packet_alloc(data, rc = size_ = size));
                }
                else
                {

                    std::unique_ptr<packet_data_t> ptr(packet_alloc(nullptr, size_ + size));
                    std::copy(data_.get(), data_.get() + size_, ptr.get());
                    std::copy(data,  data + size, ptr.get() + size_);
                    data_ = std::move(ptr);
                    size_ += size;
                    rc = size;
                }
            }

            return rc;
        }

        void Packet::Reset()
        {
            offset_ = 0;
        }

        Packet::operator const packet_data_ptr_t() const
        {
            return Data();
        }


        const packet_data_ptr_t Packet::Data() const
        {
            return data_.get() + offset_;
        }

        const collection_size_t Packet::Size() const
        {
            return size_ - offset_;
        }

        std::shared_ptr<Packet> Packet::Clone()
        {
            return std::make_shared<Packet>(*this);
        }
	}
}


