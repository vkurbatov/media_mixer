#include <memory>
#include <algorithm>

#include "packet.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("Packet", msg)

namespace mmx
{
    namespace data
    {
        Packet::Packet(int size) :
            data_(size),
            idx_(0)
        {
            DLOGT(LOG_BEGIN("Packet(%d)"), size);
        }

        Packet::Packet(const packet_data_t* data, int size) :
            data_(data, data + size),
            idx_(0)
        {
            DLOGT(LOG_BEGIN("Packet(%x, %d)"), DLOG_POINTER(data), size);
        }

        Packet::Packet(const Packet& packet) :
            data_(packet.data_),
            idx_(packet.idx_)
        {
            DLOGT(LOG_BEGIN("Packet(&%x)"), DLOG_POINTER(&packet));
        }

        Packet::Packet(Packet&& packet) :
            data_(std::move(packet.data_)),
            idx_(packet.idx_)
        {
            DLOGT(LOG_BEGIN("Packet(&&%x)"), DLOG_POINTER(&packet));

            packet.idx_ = 0;
        }

        Packet& Packet::operator=(const Packet& packet)
        {

            if (&packet != this)
            {
                DLOGT(LOG_BEGIN("operator=(&%x)"), DLOG_POINTER(&packet));

                data_ = packet.data_;
                idx_ = packet.idx_;
            }

            return *this;

        }

        Packet& Packet::operator=(Packet&& packet)
        {

            if (&packet != this)
            {

                DLOGT(LOG_BEGIN("operator=(&&%x)"), DLOG_POINTER(&packet));

                data_ = std::move(packet.data_);
                idx_ = packet.idx_;
                packet.idx_ = 0;              

            }

            return *this;

        }

        int Packet::Append(const Packet& packet)
        {
            DLOGT(LOG_BEGIN("Append(&%x)"), DLOG_POINTER(&packet));

            return Append(packet.Data(), packet.Size());
        }

        int Packet::Append(const packet_data_t* data, int size)
        {
            if (size > 0)
            {
                DLOGT(LOG_BEGIN("Append(%x, %d)"), DLOG_POINTER(data), size);

                std::copy(data, data + size, std::back_inserter(data_));
                return size;
            }
            return 0;
        }

        Packet& Packet::operator += (const Packet& packet)
        {
            DLOGT(LOG_BEGIN("operator +=(%x)"), DLOG_POINTER(&packet));

            Append(packet);
            return *this;
        }

        Packet operator + (const Packet& packet1, const Packet& packet2)
        {
            DLOGT("Packet::operator +(%x, %x)",DLOG_POINTER(&packet1), DLOG_POINTER(&packet2));

            Packet pack(packet1);
            pack+= packet2;           

            return std::move(pack);
        }

        void Packet::Seek(int idx)
        {

            DLOGT(LOG_BEGIN("Seek(%d)"),idx);

            if (idx < data_.size())
            {
                idx_ = idx;
            }

        }

        void Packet::Reset()
        {
            DLOGT(LOG_BEGIN("Reset: idx_ = %d -> 0"),idx_);

            idx_ = 0;
        }

        void Packet::Resize(int size)
        {
            DLOGT(LOG_BEGIN("Resize(%d): idx_ = %d"),size, idx_);

            if (size > 0)
            {
                data_.resize(size + idx_);
            }
        }

        packet_data_t& Packet::operator[] (int idx)
        {

            return data_[idx + idx_];
        }

        Packet::operator packet_data_t* ()
        {
            return Data() + idx_;
        }

        packet_data_t* Packet::Data(int idx)
        {

            return data_.data() + idx_ + idx;
        }

        const packet_data_t* Packet::Data(int idx) const
        {

            return data_.data() + idx_ + idx;
        }

        int Packet::Size() const
        {
            return data_.size() - idx_;
        }

        int Packet::Position() const
        {
            return idx_;
        }
    }
}
