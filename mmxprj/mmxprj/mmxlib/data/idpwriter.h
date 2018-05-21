#ifndef _MMX_DATA_I_DP_WRITER_H
#define _MMX_DATA_I_DP_WRITER_H

#include "headers/datapack.h"

namespace mmx
{
    namespace data
    {
        class IDataPacketWriter
        {

        public:

            virtual ~IDataPacketWriter(){};
            virtual int BuildPacket(int pack_id = 0) = 0;
            virtual headers::PDATA_BLOCK QueryBlock(int size) = 0;
            virtual int Commit() = 0;
            virtual int Blocks() const = 0;


        };
    }
}

#endif
