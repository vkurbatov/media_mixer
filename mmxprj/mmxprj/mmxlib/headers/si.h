#ifndef _MMX_HEADERS_SANGOMA_INTERFACE_H
#define _MMX_HEADERS_SANGOMA_INTERFACE_H

namespace mmx
{
    namespace headers
    {

        static const int SI_MAX_PYLOAD_SIZE = 512;

#pragma pack(push, 1)


        static const int SI_PASSWORD_LEN = 6;
        static const int SI_PYLOAD_SIZE = 256;

        enum sangoma_msg_t: unsigned char
        {

            SI_PASSWORD_STATUS,
            SI_LINK_STATUS,
            SI_START_PROXY,
            SI_END_PROXY

        };

        enum sangoma_link_t: unsigned char
        {

            SI_LINK_E1,
            SI_LINK_X25,
            SI_LINK_TCP

        };



        typedef struct _SANGOMA_HEADER
        {

            unsigned char   type;       // тип сообщения (sangoma_msg_t)
            unsigned short  length;     // значимая длина pyload

        }SANGOMA_HEADER, *PSANGOMA_HEADER;

        typedef struct _SANGOMA_LINK_STATUS
        {

           unsigned char    type;       // тип интерфейса связи (sangoma_link_t)
           unsigned char    num;        // номер потока
           unsigned int     status;     // бит-статус каналов связи


        }SANGOMA_LINK_STATUS, *PSANGOMA_LINK_STATUS;

        typedef struct _SANGOMA_NET_POINT
        {

            unsigned int        address;
            unsigned short      port;

        }SANGOMA_NET_POINT, *PSANGOMA_NET_POINT;

        typedef struct _SANGOMA_PROXY_INFO
        {

            SANGOMA_NET_POINT   source_a;
            SANGOMA_NET_POINT   source_b;

        }SANGOMA_PROXY_INFO,*PSANGOMA_PROXY_INFO;

        typedef struct _SANGOMA_SORM_INFO
        {

          unsigned char     channel_id;         // канал обмена
          unsigned char     sorm_id;            // номер пульта ОРМ
          unsigned short    object_id;
          unsigned short    call_id;
          unsigned char     conn_param;
          unsigned char     mcl_a;
          unsigned char     mcl_b;

        }SANGOMA_SORM_INFO, *PSANGOMA_SORM_INFO;

        static const int SI_MAX_SORM_COUNT = (SI_PYLOAD_SIZE - sizeof(SANGOMA_PROXY_INFO)) / sizeof(SANGOMA_SORM_INFO);
        static const int SI_MAX_STATUS_LINK_COUNT = (SI_PYLOAD_SIZE - sizeof(SANGOMA_PROXY_INFO)) / sizeof(SANGOMA_LINK_STATUS);

        typedef struct _SANGOMAN_PACKET
        {

            SANGOMA_HEADER  header;
            union
            {

                char data[SI_PYLOAD_SIZE];
                struct
                {
                    SANGOMA_PROXY_INFO proxy;
                    SANGOMA_SORM_INFO  sorm[SI_MAX_SORM_COUNT];
                }q_proxy;
                SANGOMA_LINK_STATUS a_link_status[SI_MAX_STATUS_LINK_COUNT];

            };

        }SANGOMA_PACKET,*PSANGOMA_PACKET;




        typedef struct _SANGOMA_MEDIA_STREAM_HEADER
        {

            unsigned short  packet_id;  // инкрементный счетчик кадров
            unsigned short  zero1;      // обязательный (0)
            unsigned short  length;     // размер pyload + длина заголовка
            unsigned short  zero2;      // обязательный (0)
            unsigned int    pid;        // reserved (устанавливать в 1)
            unsigned int    lid;        // идентификатор соединения/слота

        }SANGOMA_MEDIA_STREAM_HEADER, *PSANGOMA_MEDIA_STREAM_HEADER;

        typedef struct _SANGOMA_MEDIA_STREAM_PACKET
        {

            SANGOMA_MEDIA_STREAM_HEADER header;
            char data[SI_MAX_PYLOAD_SIZE];

        }SANGOMA_MEDIA_STREAM_PACKET,*PSANGOMA_MEDIA_STREAM_PACKET;




    }

#pragma pack(pop)

}

#endif
