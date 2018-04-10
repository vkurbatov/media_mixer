#include "dpreader.h"

namespace mmx
{
    namespace data
    {

        DataPacketReader::DataPacketReader(const headers::DATA_PACK_HEADER* data) :
            data_((const headers::DATA_PACK*)data),
            pos_(0)
        {

        }

        DataPacketReader::DataPacketReader(DataPacketReader&& dpreader) :
            data_(dpreader.data_),
            pos_(dpreader.pos_)
        {
            dpreader.data_ = nullptr;
            dpreader.pos_ = 0;
        }

        // IDataPacketReader

        headers::PDATA_BLOCK DataPacketReader::GetBlock()
        {
            headers::PDATA_BLOCK rc = nullptr;

            if (data_ != nullptr
                    && data_->header.length > headers::MIN_PACKET_SIZE
                    && data_->header.magic == headers::DATA_PACK_MAGIC
                    && *(unsigned short*)((char*)data_ + data_->header.length - sizeof(headers::DATA_PACK_MAGIC2)) == headers::DATA_PACK_MAGIC2
                    && (pos_ + sizeof(headers::DATA_PACK_HEADER)) < (data_->header.length - sizeof(headers::DATA_PACK_MAGIC2)))
            {

                rc = (headers::PDATA_BLOCK)(data_->data + pos_);
                pos_ += rc->header.length;

            }

            return rc;
        }

        void DataPacketReader::Reset()
        {
            pos_ = 0;
        }
    }
}
