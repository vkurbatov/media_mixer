#ifndef _MMX_DATA_DP_READER_H
#define _MMX_DATA_DP_READER_H

#include "idpreader.h"

namespace mmx
{
    namespace data
    {
        class DataPacketReader : public IDataPacketReader
        {

            const headers::DATA_PACK* data_;
            int pos_;

        public:

            DataPacketReader(const headers::DATA_PACK_HEADER* data);
            DataPacketReader(DataPacketReader&& dpreader);
            ~DataPacketReader();

            // IDataPacketReader

            headers::PDATA_BLOCK GetBlock() override;
            void Reset() override;


        };
    }
}

#endif
