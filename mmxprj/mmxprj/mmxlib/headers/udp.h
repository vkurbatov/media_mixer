#ifndef _MMX_HEADERS_UDP_H
#define _MMX_HEADERS_UDP_H

#include "headers/ip.h"

namespace mmx
{
    namespace headers
    {
#pragma pack(push,1)


        typedef struct _UDPHEADER
        {
            unsigned short port_src;
            unsigned short port_dst;
            unsigned short length;
            unsigned short crc;
        }UDPHEADER,*PUDPHEADER;

        typedef struct _UDPIP4
        {
            IP4HEADER	ip4_header;
            UDPHEADER   udp_header;
        }UDPIP4,*PUDPIP4;

        typedef struct _UDPPACKET
        {
            IP4HEADER	ip4_header;
            UDPHEADER   udp_header;
            unsigned char data[1];
        }UDPPACKET,*PUDPPACKET;


#pragma pack(pop)
    }
}

#endif
