#ifndef _MMX_HEADERS_IP_H
#define _MMX_HEADERS_IP_H

namespace mmx
{
    namespace headers
    {
#pragma pack(push,1)

        typedef struct _IP4HEADER
        {
            unsigned char  len : 4;
            unsigned char  ver : 4;
            unsigned char  tos;
            unsigned short length;
            unsigned short id;
            unsigned short flgs_offset;
            unsigned char  ttl;
            unsigned char  protocol;
            unsigned short xsum;
            unsigned long  src;
            unsigned long  dest;
            //unsigned short params[4];
            //unsigned char  data[0];
        }IP4HEADER, *PIP4HEADER;

#pragma pack(pop)
    }
}

#endif
