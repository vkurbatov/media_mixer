#ifndef _MMX_HEADERS_RTP_H
#define _MMX_HEADERS_RTP_H

namespace mmx
{
    namespace headers
    {

            static const unsigned short RTP_VERSION = 2;


#pragma pack(push, 1)

        typedef struct _RTP_HEADER
        {

            unsigned short  ver:2;              // версия протокола = 2
            unsigned short  padding:1;          // применяется заполнение
            unsigned short  extension:1;        // есть расширение заголовка
            unsigned short  csrc_count:4;       // количество CSRC
            unsigned short  marker:1;           // маркировка прикладного профиля
            unsigned short  pyload_type:7;      // тип данных (PCMA = 0x08)
            unsigned int    timestamp;          // метка времени
            unsigned int    ssrc;               // источник синхронизации

        }RTP_HEADER, *PRTP_HEADER;

        typedef struct _RTP_EXTENSION_HEADER
        {
            unsigned short  header;
            unsigned short  ehl;

        }RTP_EXTENSION_HEADER,*PRTP_EXTENSION_HEADER;

        typedef struct _RTP_EXTENSION
        {

            RTP_EXTENSION_HEADER  header;
            unsigned int          param[1];

        }RTP_EXTENSION,*PRTP_EXTENSION;


        typedef struct _RTP_PACKET
        {

            RTP_HEADER header;
            union
            {
                  unsigned int  csrc[1];
                  char          data[1];
            };

        }RTP_PACKET, *PRTP_PACKET;

    }

#pragma pack(pop)

}

#endif
