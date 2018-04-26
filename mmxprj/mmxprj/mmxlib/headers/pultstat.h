#ifndef _MMX_HEADERS_PULT_STAT_H
#define _MMX_HEADERS_PULT_STAT_H

namespace mmx
{
    namespace headers
    {

        const unsigned short    PULT_STAT_MAGIC = 0x1400;

#pragma pack(push, 1)

        typedef struct _PULT_STAT
        {

            unsigned short	magic;              // обязательная сигнатура
            unsigned short  online_conn;        // количество подключенных клиентов
            unsigned short  total_conn;         // всего подключалось клиентов
            unsigned short  errors;             // отключение клиентов с ошибкой
            unsigned int    send_packets;       // передано пакетов
            unsigned int    recv_packets;       // получено пакетов

        }PULT_STAT,*PPULT_STAT;

#pragma pack(pop)

    }
}

#endif
