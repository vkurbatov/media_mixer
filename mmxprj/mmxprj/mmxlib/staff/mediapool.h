#ifndef _MMX_STAFF_PACK_POOL_H
#define _MMX_STAFF_PACK_POOL_H

#include "packet.h"
#include <queue>

#include "headers/media.h"
#include "datapacket.h"

namespace mmx
{
    namespace staff
    {
        const int DEFAULT_PACK_SIZE = 0x10000;
        const int DEFAULT_BLOCKS = 5;
        //const int MIN_PACK_SIZE = 0x1000;
        //const int MAX_PACK_SIZE = 0x80000;
        //const int MIN_BLOCKS = 1;
        //const int MAX_BLOCKS = 10;

        class MediaPool
        {
            static int s_pack_id_;
        public:
            MediaPool(int pack_size = DEFAULT_PACK_SIZE, int max_blocks = DEFAULT_BLOCKS);
            ~MediaPool();

        };
    }
}

#endif
