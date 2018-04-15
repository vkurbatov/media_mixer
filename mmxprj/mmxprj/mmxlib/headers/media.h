#ifndef _MMX_HEADERS_MEDIA_H
#define _MMX_HEADERS_MEDIA_H

namespace mmx
{
    namespace headers
    {
    	
        const unsigned short    MEDIA_MAGIC = 0x4444;
        const unsigned short    MEDIA_VERSION = 1;

#pragma pack(push,1)

        typedef struct _NET_POINT
        {
            unsigned int    address;        // IP-адрес
            unsigned short  port;           // UDP/TCP порт

        }NET_POINT,*PNET_POINT;

        typedef struct _NET_PAIR
        {

            NET_POINT   source;             // точка подключения источника
            NET_POINT   destination;        // точка подключения получателя

        }NET_PAIR,*PNET_PAIR;


        typedef struct _MEDIA_HEADER
        {
        
            unsigned short	magic;          // сигнатура пакета
            unsigned short  packet_id;      // порядковый номер пакета

            unsigned short	length;         // длина медиаданных + размер заголовка
            unsigned char   ver;            // версия медиапакета
            unsigned char   reserved;       // зарезервировано

            NET_PAIR        net_points;     // коммуникационная точка транзакции
        	
            unsigned int    timestamp;      // метка времени (количество миллисекунд с начала суток)

            
        }MEDIA_HEADER,*PMEDIA_HEADER;

        typedef struct MEDIA_DATA
        {

            MEDIA_HEADER    header;
            char            media[1];

        }MEDIA_DATA,*PMEDIA_DATA;


        const unsigned short    MEDIA_SAMPLE_MAGIC = 0x4441;
        //

        typedef struct _MEDIA_SAMPLE_HEADER
        {

            unsigned short  magic;          // сигнатура семпла
            unsigned short  packet_id;      // порядковый номер пакета

            unsigned short  length;         // длина медиапакета

            NET_PAIR        net_points;     // коммуникационная точка транзакции

            unsigned int    timestamp;      // метка времени (количество миллисекунд с начала суток)
            unsigned int    rtp_timestamp;  // метка времени (метка времени RTP)

            unsigned int    sample_id;      // ssrc RTP

            unsigned short  media_type;     // тип медиаданных (PMCA, PMCU, G722 и т.д.)

        }MEDIA_SAMPLE_HEADER, *PMEDIA_SAMPLE_HEADER;

        typedef struct _MEDIA_SAMPLE
        {
            MEDIA_SAMPLE_HEADER header;
            char                media[1];

        }MEDIA_SAMPLE, *PMEDIA_SAMPLE;



#pragma pack(pop)

    }
}

#endif
