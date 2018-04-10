#ifndef _MMX_DATA_I_DP_HEADER_H
#define _MMX_DATA_I_DP_HEADER_H

#include "headers/datapack.h"

namespace mmx
{
    namespace data
    {
        class IDataPacketHeader
        {

        public:
            virtual ~IDataPacketHeader() {};
            virtual bool IsValid() const = 0;
            virtual headers::PDATA_PACK Header() = 0;
            virtual const headers::DATA_PACK* Header() const = 0;
            virtual bool IsEmpty() const = 0;

        };
    }
}

#endif
