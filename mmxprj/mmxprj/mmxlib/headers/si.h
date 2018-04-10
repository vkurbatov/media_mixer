#ifndef _MMX_HEADERS_SANGOMA_INTERFACE_H
#define _MMX_HEADERS_SANGOMA_INTERFACE_H

namespace mmx
{
    namespace headers
    {

        static const int MAX_PYLOAD_SIZE = 512;

#pragma pack(push, 1)

        typedef struct _SANGOMA_HEADER
        {

            unsigned short  packet_id;  // инкрементный счетчик кадров
            unsigned short  zero1;      // зарезервировано (0)
            unsigned short  length;     // размер pyload + дина заголовка
            unsigned short  zero2;      // зарезервировано (0)
            unsigned int    pid;        // reserved
            unsigned int    lid;        // идентификатор соединения/слота

        }SANGOMA_HEADER, *PSANGOMA_HEADER;

        typedef struct _SANGOMA_PACKET
        {
            SANGOMA_HEADER header;
            char data[MAX_PYLOAD_SIZE];
        }SANGOMA_PACKET,*PSANGOMA_PACKET;
    }

#pragma pack(pop)

}

#endif
