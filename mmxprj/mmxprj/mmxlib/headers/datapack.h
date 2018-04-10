#ifndef _MMX_HEADERS_DATA_PACK_H
#define _MMX_HEADERS_DATA_PACK_H

namespace mmx
{
    namespace headers
    {
    	

#pragma pack(push,1)

        typedef struct _DATA_PACK_HEADER
        {
        
            unsigned short	magic;			// обязательная сигнатура
            unsigned short	length;			// общее количество блока данных + заголовок
            unsigned short	pack_id;		// порядковый номер пакета данных
            unsigned short	reserved;		//
			unsigned short	block_count;	// количество добавленных блоков
			
            
        }DATA_PACK_HEADER,*PDATA_PACK_HEADER;

        typedef struct _DATA_BLOCK_HEADER
        {

            unsigned short length;          // длина блока + заголовок
            unsigned short block_id;        // порядковый номер блока данных

        }DATA_BLOCK_HEADER, *PDATA_BLOCK_HEADER;

        typedef struct _DATA_BLOCK
        {
            DATA_BLOCK_HEADER header;
            char data[1];
        }DATA_BLOCK, *PDATA_BLOCK;
        
        typedef struct _DATA_PACK
        {
        
            DATA_PACK_HEADER   header;
            union
            {
                char                data[1];
                DATA_BLOCK_HEADER   first;
            };

        }DATA_PACK,*PDATA_PACK;

#pragma pack(pop)

        const unsigned short    DATA_PACK_MAGIC = 0x55AA;
        const unsigned short    DATA_PACK_MAGIC2 = ~DATA_PACK_MAGIC;
        const unsigned int      MAX_PACKET_SIZE = 0xFFFF;
        const unsigned int      MIN_PACKET_SIZE = sizeof(DATA_PACK_HEADER) + sizeof(DATA_PACK_MAGIC2);
        const unsigned int      MAX_PACKET_PYLOAD_SIZE = MAX_PACKET_SIZE - MIN_PACKET_SIZE;

    }
}

#endif
