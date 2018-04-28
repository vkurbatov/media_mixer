#ifndef _MMX_DATA_DATAPACKET_H
#define _MMX_DATA_DATAPACKET_H

#include <vector>

#include "dpreader.h"
#include "dpwriter.h"
#include "idpheader.h"

namespace mmx
{
    namespace data
    {
        class DataPacket : public IDataPacketHeader, public IDataPacketWriter, public IDataPacketReader
        {
            std::vector<unsigned char> data_;
            DataPacketWriter dp_writer_;
            DataPacketReader dp_reader_;

        public:
            DataPacket(int size = 0x10000);
            DataPacket(DataPacket&& dp);
            ~DataPacket() override;


            // IDataPacketHeader

            bool IsValid() const override;
            headers::PDATA_PACK Header() override;
            const headers::DATA_PACK* Header() const override;
            bool IsEmpty() const override;

            // IDataPacketWriter

            int BuildPacket(int pack_id = 0) override;
            headers::PDATA_BLOCK QueryBlock(int size) override;
            int Commit() override;

            // IdataPacketReader

            headers::PDATA_BLOCK GetBlock() override;
            void Reset() override;

        };
    }
}

#endif
