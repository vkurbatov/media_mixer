#ifndef _MMX_HEADERS_RTP_H
#define _MMX_HEADERS_RTP_H

namespace mmx
{
    namespace headers
    {

        static const unsigned short RTP_VERSION = 2;

        enum MEDIA_TYPES : unsigned char
        {

            MA_PCMU     = 0,
            MA_G711U    = 0,
            MA_1016     = 1,
            MA_G721     = 2,
            MA_GSM      = 3,
            MA_G723     = 4,
            MA_DVI4_8   = 5,
            MA_DVI4_16  = 6,
            MA_LPC      = 7,
            MA_PCMA     = 8,
            MA_G711A    = 8,
            MA_G722     = 9,
            MA_L16_C2   = 10,
            MA_L16_C1   = 11,
            MA_QCELP    = 12,
            MA_CN       = 13,
            MA_NPA      = 14,
            MA_G728     = 15,
            MA_DVI4_11  = 16,
            MA_DVI4_22  = 17,
            MA_G729     = 18,
            MV_CELB     = 25,
            MV_JPEG     = 26,
            MV_NV       = 28,
            MV_H261     = 31,
            MV_MPV      = 32,
            MVA_MP2T    = 33,
            MV_H263     = 34

        };


#pragma pack(push, 1)

        typedef struct _RTP_HEADER
        {

            unsigned short  ver:2;              // версия протокола = 2
            unsigned short  padding:1;          // применяется заполнение
            unsigned short  extension:1;        // есть расширение заголовка
            unsigned short  csrc_count:4;       // количество CSRC
            unsigned short  marker:1;           // маркировка прикладного профиля
            unsigned short  pyload_type:7;      // тип данных (PCMA = 0x08)
            unsigned int    timestamp;          // метка времени (миллисекунды)
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
