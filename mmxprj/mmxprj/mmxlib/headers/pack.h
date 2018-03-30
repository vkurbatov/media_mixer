#ifndef _MMX_HEADERS_PACK_H
#define _MMX_HEADERS_PACK_H

namespace mmx
{
    namespace headers
    {
    	
        const unsigned short    PACK_MAGIC = 0x5555;

#pragma pack(push,1)

        typedef struct _MEDIA_PACK_HEADER
        {
        
			unsigned short	magic;			// обязательная сигнатура			
			unsigned short	pack_id;		// порядковый номер пакета
			unsigned short	proto_id;		// 0
			unsigned short	block_count;	// количество добавленных блоков
			unsigned short	total;			// общее количество блоков данных
			
            
        }PACK_HEADER,*PPACK_HEADER;
        
        typedef struct _MEDIA_PACK
        {
        
			PACK_HEADER pack_header;
			char		data[1];
			           
        }MEDIA_PACK,*PMEDIA_PACK;

#pragma pack(pop)

    }
}

#endif
