#include "datapacket.h"

namespace mmx
{
    namespace data
    {
        DataPacket::DataPacket(int size) :
            data_(size <= 0 ? headers::MAX_PACKET_SIZE :
                              sizeof(headers::DATA_PACK_HEADER) + sizeof(headers::DATA_PACK_MAGIC2) + size),
            dp_writer_(data_.data(),data_.size()),
            dp_reader_((headers::PDATA_PACK_HEADER)data_.data())
        {

        }

        DataPacket::DataPacket(DataPacket&& dp) :
            data_(std::move(dp.data_)),
            dp_writer_(std::move(dp.dp_writer_)),
            dp_reader_(std::move(dp.dp_reader_))
        {

        }


        // IDataPacketHeader

        bool DataPacket::IsValid() const
        {
            headers::PDATA_PACK_HEADER header = (headers::PDATA_PACK_HEADER)data_.data();
            return header != nullptr
                    && data_.size() >= sizeof(headers::DATA_PACK_HEADER)
                    && *(unsigned short*)((char*)data_.data() + header->length - sizeof(headers::DATA_PACK_MAGIC2)) == headers::DATA_PACK_MAGIC2
                    && header->magic == headers::DATA_PACK_MAGIC;
        }

        headers::PDATA_PACK DataPacket::Header()
        {
            return (headers::PDATA_PACK)data_.data();
        }

        const headers::DATA_PACK* DataPacket::Header() const
        {
            return (headers::PDATA_PACK)data_.data();
        }

        bool DataPacket::IsEmpty() const
        {
            return !IsValid() || Header()->header.block_count == 0;
        }

        // IDataPacketWriter

        int DataPacket::BuildPacket(int pack_id)
        {
            dp_reader_.Reset();
            return dp_writer_.BuildPacket(pack_id);
        }

        headers::PDATA_BLOCK DataPacket::QueryBlock(int size)
        {
            return dp_writer_.QueryBlock(size);
        }

        int DataPacket::Commit()
        {
            return dp_writer_.Commit();
        }

        // IdataPacketReader

        headers::PDATA_BLOCK DataPacket::GetBlock()
        {
            return dp_reader_.GetBlock();
        }

        void DataPacket::Reset()
        {
            return dp_reader_.Reset();
        }

    }
}