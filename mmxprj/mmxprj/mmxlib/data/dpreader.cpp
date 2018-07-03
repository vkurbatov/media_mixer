/*  Объект: Библиотека медиа модуля (mmx).
 *
 *  Сущность: Подсистема данных (data)
 *
 *  Описание:
 *
 *  Класс DataPacketReader является оберткой для внутреннего протокола обмена
 *  данными между микросервисами. Объеденяет интерфейсы чтения блоков
 *  данных и их формирования. По сути является аггрегатором классов
 *  DataPackReader и DataPackWriter.
 *
 */


#include "dpreader.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("DataPacketReader", msg)

namespace mmx
{
    namespace data
    {

        DataPacketReader::DataPacketReader(const headers::DATA_PACK_HEADER* data) :
            data_((const headers::DATA_PACK*)data),
            pos_(0)
        {
            DLOGT(LOG_BEGIN("DataPacketReader(%d)"), DLOG_POINTER(data));
        }

        DataPacketReader::DataPacketReader(DataPacketReader&& dpreader) :
            data_(dpreader.data_),
            pos_(dpreader.pos_)
        {
            dpreader.data_ = nullptr;
            dpreader.pos_ = 0;
            DLOGT(LOG_BEGIN("DataPacketReader(&&%d)"), DLOG_POINTER(&dpreader));
        }

        DataPacketReader::~DataPacketReader()
        {
            DLOGT(LOG_BEGIN("~DataPacketReader()"));
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
                DLOGT(LOG_BEGIN("GetBlock(): return PDATA_BLOCK{%x}, length = %d)"), DLOG_POINTER(rc), rc->header.length);

            }

            return rc;
        }

        void DataPacketReader::Reset()
        {
            DLOGT(LOG_BEGIN("Reset(): pos_ = %d -> 0"), pos_);
            pos_ = 0;
        }
    }
}
