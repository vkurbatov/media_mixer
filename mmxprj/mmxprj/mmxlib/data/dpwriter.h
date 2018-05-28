#ifndef _MMX_DATA_DP_WRITER_H
#define _MMX_DATA_DP_WRITER_H

#include "idpwriter.h"

namespace mmx
{
    namespace data
    {
        class DataPacketWriter : public IDataPacketWriter
        {
            headers::PDATA_PACK data_;
            int size_;
            bool build_;
            bool query_;

        public:

            DataPacketWriter(void* data, int size);
            DataPacketWriter(DataPacketWriter&& dpwriter);

            // IDataPacketWriter

            int BuildPacket(int pack_id = 0) override;
            headers::PDATA_BLOCK QueryBlock(int size) override;
            int Write(const void* data, int size) override;
            int Commit() override;
            int Blocks() const override;
        };
    }
}

#endif
