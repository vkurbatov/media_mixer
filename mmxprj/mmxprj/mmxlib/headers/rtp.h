#ifndef _MMX_HEADERS_RTP_H
#define _MMX_HEADERS_RTP_H

namespace mmx
{
    namespace headers
    {

        static const unsigned short RTP_VERSION = 2;

        enum media_profile_t : unsigned char
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

        typedef struct _MEDIA_PROFILE_INFO
        {
            int             frequency;
            unsigned char   cnannels;
            short           frame_size;
            short           default_frame_size;
        }MEDIA_PROFILE_INFO, *PMEDIA_PROFILE_INFO;

        static const MEDIA_PROFILE_INFO media_profiles_info_table[] =
        {
/* 0  */    { 8000,     1,      -1,     20},
/* 1  */    { 8000,     1,      -1,     20},
/* 2  */    { 8000,     1,      -1,     20},
/* 3  */    { 8000,     1,      -1,     20},
/* 4  */    { 8000,     1,      30,     30},
/* 5  */    { 8000,     1,      -1,     20},
/* 6  */    {16000,     1,      -1,     20},
/* 7  */    { 8000,     1,      -1,     20},
/* 8  */    { 8000,     1,      -1,     20},
/* 9  */    { 8000,     1,      -1,     20},
/* 10 */    {44100,     2,      -1,     20},
/* 11 */    {44100,     1,      -1,     20},
/* 12 */    { 8000,     2,      20,     20},
/* 13 */    {   -1,     0,      -1,     01},
        };

        static const int MEDIA_PROFILE_COUNT = sizeof(media_profiles_info_table) / sizeof(MEDIA_PROFILE_INFO) - 1;

        typedef struct _RTP_HEADER
        {

            unsigned short  csrc_count:4;       // количество CSRC
            unsigned short  extension:1;        // есть расширение заголовка
            unsigned short  padding:1;          // применяется заполнение
            unsigned short  ver:2;              // версия протокола = 2

            unsigned short  pyload_type:7;      // тип данных (PCMA = 0x08)
            unsigned short  marker:1;           // маркировка прикладного профиля

            unsigned short  packet_id;          // порядковый номер
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
