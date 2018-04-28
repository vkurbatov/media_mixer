#include "dpwriter.h"

#include <errno.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("DataPacketWriter", msg)

#define MAGIC_STX(begin, off) (*(unsigned short*)((char*)(begin) + ((headers::PDATA_PACK_HEADER)(begin))->length - sizeof(headers::DATA_PACK_MAGIC2) + (off)))

namespace mmx
{
    namespace data
    {

        DataPacketWriter::DataPacketWriter(void* data, int size) :
            data_((headers::PDATA_PACK)data),
            size_(size > headers::MAX_PACKET_SIZE ? headers::MAX_PACKET_SIZE : size),
            build_(false),
            query_(false)
        {
            DLOGT(LOG_BEGIN("DataPacketWriter(%x, %d)"), DLOG_POINTER(data), size);
        }

        DataPacketWriter::DataPacketWriter(DataPacketWriter&& dpwriter) :
            data_(dpwriter.data_),
            size_(dpwriter.size_),
            build_(dpwriter.build_),
            query_(dpwriter.query_)
        {
            dpwriter.data_ = nullptr;
            dpwriter.size_ = 0;
            dpwriter.build_ = dpwriter.query_ = false;

            DLOGT(LOG_BEGIN("DataPacketWriter(&&%x)"), DLOG_POINTER(&dpwriter));
        }

        int DataPacketWriter::BuildPacket(int pack_id)
        {
            int rc = -EINVAL;

            if (data_ != nullptr
                    && size_ >= sizeof(headers::MIN_PACKET_SIZE + sizeof(headers::DATA_BLOCK_HEADER)))
            {

                data_->header.magic = headers::DATA_PACK_MAGIC;
                data_->header.pack_id = pack_id == 0 && build_ == true ? data_->header.pack_id : pack_id;
                //data_->header.pack_id += (unsigned short)(data_->header.pack_id == 0);
                data_->header.reserved = 0;
                data_->header.block_count = 0;
                data_->header.length = headers::MIN_PACKET_SIZE;

                MAGIC_STX(data_,0) = headers::DATA_PACK_MAGIC2;

                build_ = true;
                query_ = false;

                rc = size_ - sizeof(headers::DATA_PACK_HEADER);

                DLOGT(LOG_BEGIN("BuildPacket(%d): Build packet succes"), pack_id);

            }
            else
            {
                DLOGT(LOG_BEGIN("BuildPacket(%d): Err: Invalid Packet"), pack_id);
            }


            return rc;
        }

        headers::PDATA_BLOCK DataPacketWriter::QueryBlock(int size)
        {
            headers::PDATA_BLOCK rc = nullptr;

            if (build_)
            {
                size += sizeof(headers::DATA_BLOCK_HEADER);
                int elapse_size = size_ - data_->header.length;

                if (elapse_size >= size)
                {

                    rc = (headers::PDATA_BLOCK)((char*)data_
                                                + data_->header.length
                                                - sizeof(headers::DATA_PACK_MAGIC2));

                    rc->header.block_id = 0;
                    rc->header.length = size;

                    MAGIC_STX(data_, size) = headers::DATA_PACK_MAGIC2;

                    query_ = true;

                    DLOGT(LOG_BEGIN("QueryBlock(%d): Query block succes"), size);

                }
                else
                {
                    errno = ENOMEM;
                    DLOGT(LOG_BEGIN("QueryBlock(%d): Err: Not enought memory"), size);
                }
            }
            else
            {
                errno = EINVAL;
                DLOGT(LOG_BEGIN("QueryBlock(%d): Err: Packet no build"), size);
            }

            return rc;

        }

        int DataPacketWriter::Commit()
        {
            int rc = -EINVAL;

            if (query_)
            {

                headers::PDATA_BLOCK block = (headers::PDATA_BLOCK)((char*)data_
                                             + data_->header.length
                                             - sizeof(headers::DATA_PACK_MAGIC2));

                block->header.block_id = ++data_->header.block_count;
                rc = block->header.length;
                data_->header.length += rc;

                *(unsigned short*)((char*)block + block->header.length) = headers::DATA_PACK_MAGIC2;

                query_ = false;

                DLOGT(LOG_BEGIN("Commit: Success  commit block#%d"), data_->header.block_count);

            }

            return rc;
        }

    }
}
