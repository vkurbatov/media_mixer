#ifndef _MMX_HEADERS_ORDER_645_2_H
#define _MMX_HEADERS_ORDER_645_2_H

namespace mmx
{
    namespace headers
    {

#pragma pack(push, 1)

        static const unsigned char ORDER_645_2_MAGIC = 0xDD;
        static const unsigned short ORDER_645_2_PACKET_SIZE = 1039;

        typedef struct _ORDER_645_2_HEADER
        {

 /*+0*/     unsigned char   magic;              // преамбула, вcегда = 0xDD
 /*+1*/     unsigned char   packet_id;          // циклический номер передачи [0x00..0xFF, 0x00..]
 /*+2*/     unsigned char   sorm_id;            // номер ТС ОРМ
 /*+3*/     unsigned short  object_id;          // условный номер объекта контроля
 /*+5*/     unsigned short  call_id;            // номер вызыова
 /*+7*/     unsigned char   conn_param;         // параметры связи
 /*+8*/     unsigned char   mcl_a;              // номер КСЛ-A
 /*+9*/     unsigned char   mcl_b;              // номер КСЛ-B
 /*+10*/    unsigned int    conn_flag:8;        // признак состояния передачи: 0 - есть ПД, 1 - ПД завершена
 /*+11*/    unsigned int    block_number:24;    // порядковый номер блока данных в соединении
 /*+14*/

        }ORDER_645_2_HEADER,*PORDER_645_2_HEADER;

        static const unsigned short ORDER_645_2_MAX_DATA_SIZE = ORDER_645_2_PACKET_SIZE - sizeof(ORDER_645_2_HEADER);

        typedef struct _ORDER_645_2_PACKET
        {

            ORDER_645_2_HEADER  header;
            char                data[ORDER_645_2_MAX_DATA_SIZE];

        }ORDER_645_2_PACKET,*PORDER_645_2_PACKET;

        typedef struct _ORM_INFO_HEADER
        {
            unsigned short      size_a;
            unsigned short      size_b;
            ORDER_645_2_HEADER  order_header;

        }ORM_INFO_HEADER,*PORM_INFO_HEADER;

        static const unsigned short ORDER_645_2_TOTAL_PACKET_SIZE = sizeof(ORM_INFO_HEADER) + ORDER_645_2_MAX_DATA_SIZE;

        typedef struct _ORM_INFO_PACKET
        {

            ORM_INFO_HEADER     header;
            char                data[1];

        }ORM_INFO_PACKET,*PORM_INFO_PACKET;

#pragma pack(pop)

    }

}

#endif
