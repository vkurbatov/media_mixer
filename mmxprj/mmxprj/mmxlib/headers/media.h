#ifndef _MMX_HEADERS_MEDIA_H
#define _MMX_HEADERS_MEDIA_H

namespace mmx
{
    namespace headers
    {
    	
        const unsigned short    MEDIA_MAGIC = 0x4444;
        const unsigned short    MEDIA_VERSION = 1;

#pragma pack(push,1)

        typedef struct _MEDIA_HEADER
        {
        
            unsigned short	magic;          // сигнатура пакета
            unsigned short  packet_id;      // порядковый номер пакета

            unsigned short	length;         // длина медиаданных + заголовок
            unsigned char   ver;            // версия медиапакета
            unsigned char   reserved;       // зарезервировано
        	
            unsigned int	addr_src;       // IP-адрес источника
            unsigned int	addr_dst;       // IP-адрес получателя
         	
            unsigned short	port_src;       // порт источника
            unsigned short	port_dst;       // порт получателя

            unsigned int    sec;            // время формирования пакета (секунды)
            unsigned int    usec;           // время формирования пакета (микросекунды)

            
        }MEDIA_HEADER,*PMEDIA_HEADER;

        typedef struct MEDIA_DATA
        {

            MEDIA_HEADER    header;
            char            media[1];

        }MEDIA_DATA,*PMEDIA_DATA;

#pragma pack(pop)

    }
}

#endif
