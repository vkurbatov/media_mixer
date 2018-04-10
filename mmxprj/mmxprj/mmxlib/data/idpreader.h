#ifndef _MMX_DATA_I_DP_READER_H
#define _MMX_DATA_I_DP_READER_H

#include "headers/datapack.h"

namespace mmx
{
    namespace data
    {
        class IDataPacketReader
        {

        public:
            virtual ~IDataPacketReader() {};
            virtual headers::PDATA_BLOCK GetBlock() = 0;
            virtual void Reset() = 0;


        };
    }
}

#endif
