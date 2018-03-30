#include "datapacket.h"

namespace mmx
{
    namespace staff
    {

        DataPacket::DataPacket(Packet&& packet) :
            packet_(std::move(packet)),
            offset_(0)
        {

        }

        DataPacket::DataPacket(const Packet& packet) :
            packet_(packet),
            offset_(0)
        {

        }

        DataPacket::DataPacket(int size) :
            packet_(size),
            offset_(0)
        {

        }

        DataPacket::~DataPacket()
        {

        }

        headers::PDATA_PACK  DataPacket::Init(int pack_id)
        {
            headers::PDATA_PACK rc = nullptr;

            if (packet_.Size() >= sizeof(headers::DATA_PACK_HEADER))
            {

                rc = packet_.Cast<headers::DATA_PACK>();
                rc->header.magic = headers::DATA_PACK_MAGIC;
                rc->header.pack_id = pack_id;
                rc->header.reserved = 0;
                rc->header.block_count = 0;
                rc->header.length = sizeof(headers::DATA_PACK_HEADER);

                offset_ = sizeof(headers::DATA_PACK_HEADER);

            }

            return rc;
        }

        bool DataPacket::IsValid() const
        {
            return packet_.Size() > sizeof(headers::DATA_PACK_HEADER)
                    && (packet_.Cast<headers::DATA_PACK>())->header.magic == headers::DATA_PACK_MAGIC;
        }

        DataPacket::operator headers::PDATA_PACK()
        {
            return Data();
        }

        headers::PDATA_PACK DataPacket::Data()
        {
            return IsValid() ? packet_.Cast<headers::DATA_PACK>() : nullptr;
        }

        const headers::DATA_PACK* DataPacket::Data() const
        {

            return IsValid() ? packet_.Cast<headers::DATA_PACK>() : nullptr;
        }

        headers::PDATA_BLOCK DataPacket::QueryData(int size)
        {
            headers::PDATA_BLOCK rc = nullptr;

            headers::PDATA_PACK header = packet_.Cast<headers::DATA_PACK>();


            if (IsValid() && size>0)
            {
                size += sizeof(headers::DATA_BLOCK_HEADER);

                int new_size = size + (int)header->header.length;

                if (packet_.Size() >= new_size && new_size < MAX_PACK_SIZE)
                {
                    rc = packet_.Cast<headers::DATA_BLOCK>(header->header.length);

                    rc->header.block_id = ++header->header.block_count;
                    rc->header.length =  size;

                    header->header.length += (rc->header.length =  size);
                }
            }

            return rc;
        }

        headers::PDATA_BLOCK DataPacket::Next()
        {
            headers::PDATA_BLOCK rc = nullptr;

            headers::PDATA_PACK header = packet_.Cast<headers::DATA_PACK>();

            if (IsValid())
            {

                if (offset_ < header->header.length)
                {
                    rc = packet_.Cast<headers::DATA_BLOCK>(offset_);
                    offset_ += rc->header.length;
                }

            }

            return rc;
        }

        void DataPacket::Reset()
        {
            offset_ = sizeof(headers::DATA_PACK_HEADER);
        }


        int DataPacket::Count() const
        {
            auto hdr = Data();

            int rc = -1;

            if (hdr != nullptr)
            {
                rc = hdr->header.block_count;
            }

            return rc;
        }

        int DataPacket::Size() const
        {
            auto hdr = Data();

            int rc = -1;

            if (hdr != nullptr)
            {
                rc = hdr->header.length - sizeof(hdr->header);
            }

            return rc;
        }

        bool DataPacket::IsEmpty() const
        {
            return Count() <= 0;
        }

    }
}
