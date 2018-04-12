#ifndef _MMX_HEADERS_IP_H
#define _MMX_HEADERS_IP_H

namespace mmx
{
    namespace headers
    {

        static const int MIN_HEADER_LEN = 5;
        static const int MAX_HEADER_LEN = 15;
        static const int IP4_VERSION = 4;
        static const unsigned char MIN_IP_HEADER_START = 0x45;
        static const unsigned char MAX_IP_HEADER_START = MIN_IP_HEADER_START + 10;
#pragma pack(push,1)

        typedef struct _IP4HEADER
        {
            unsigned char  hlen : 4;
            unsigned char  ver : 4;
            unsigned char  tos;
            unsigned short length;
            unsigned short id;
            unsigned short off_lo : 5;
            unsigned short f_M : 1;
            unsigned short f_D : 1;
            unsigned short f_null : 1;
            unsigned short off_hi: 8;
            unsigned char  ttl;
            unsigned char  protocol;
            unsigned short xsum;
            unsigned int  src;
            unsigned int  dest;
            //unsigned short params[4];
            //unsigned char  data[0];

        }IP4HEADER, *PIP4HEADER;


        typedef struct _IP4PACKET
        {
            IP4HEADER       header;
            unsigned char   data[1];
        }IP4PACKET, *PIP4PACKET;

#pragma pack(pop)
    }
}

#endif
