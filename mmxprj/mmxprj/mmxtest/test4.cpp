#include "defines.h"
#include <iostream>

#include <cstring>

#include "mmxlib/sniffers/ipsniffer.h"

#include <netdb.h>

#define TEST_DATA_SIZE 16

namespace mmxtest
{
    int test4()
    {
        union
        {
            mmx::headers::IP4PACKET fragment1;
            char raw1[1600];
        };

        union
        {
            mmx::headers::IP4PACKET fragment2;
            char raw2[1600];
        };

        union
        {
            mmx::headers::IP4PACKET fragment3;
            char raw3[1600];
        };

        union
        {
            mmx::headers::IP4PACKET fragment4;
            char raw4[1600];
        };

        union
        {
            mmx::headers::IP4PACKET fragment5;
            char raw5[1600];
        };

        mmx::headers::PIP4PACKET fragments[] =
        {
            &fragment5,
            &fragment1,
            &fragment4,
            &fragment2,
            &fragment3
        };

        std::memset(raw1, 0, sizeof(raw1));
        std::memset(raw2, 0, sizeof(raw2));
        std::memset(raw3, 0, sizeof(raw3));
        std::memset(raw4, 0, sizeof(raw4));
        std::memset(raw5, 0, sizeof(raw5));

        fragment1.header.src = ::htonl(1);;
        fragment1.header.dest = ::htonl(2);;
        fragment1.header.ver = mmx::headers::IP4_VERSION;
        fragment1.header.hlen = mmx::headers::MIN_HEADER_LEN;
        fragment1.header.length = ::htons(sizeof(mmx::headers::IP4HEADER) + TEST_DATA_SIZE);
        fragment1.header.id = ::htons(1234);
        fragment1.header.protocol = IPPROTO_IP;
        fragment1.header.ttl = 64;

        fragment2 = fragment1;

        fragment2.header.off_hi = TEST_DATA_SIZE >> 3;
        fragment1.header.f_M = 1;

        fragment5 = fragment3 = fragment1;
        fragment4 = fragment2;      

        fragment4.header.id = (fragment3.header.id += ::htons(1));
        fragment5.header.id = (fragment4.header.id + ::htons(1));
        fragment5.header.f_M = 0;

        for (int i = 0; i < TEST_DATA_SIZE; i++)
        {

            fragment1.data[i] = i;
            fragment2.data[i] = TEST_DATA_SIZE + i;
            fragment3.data[i] = TEST_DATA_SIZE * 2 - i;
            fragment4.data[i] = TEST_DATA_SIZE - i;
            fragment5.data[i] = -i;

        }

        mmx::sniffers::IPPacketPool packet_pool;
        mmx::sniffers::IPSniffer sniffer_(packet_pool);

        char buffer[10000];
        char* p = buffer + 4;
        int rc = 4;

        //std::memset(buffer, 0, 4);

        for (auto f : fragments)
        {

            f->header.xsum = mmx::sniffers::IPPacket::CheckSumm(f, f->header.hlen * 4);

            int len = ::ntohs(f->header.length);
            std::memcpy(p, f, len);
            p += len + 4;
            rc += len + 4;
        }


        int len = rc;
        int part_size = len;
        int bad = 0;

        p = buffer;

        while (len > 0)
        {
            part_size = std::min(len, part_size);

            rc = sniffer_.PutStream(p, part_size);

            if (rc < 0)
            {
               std::cout << "Error fragmen sniffer. rc = " << rc << std::endl;

               sniffer_.Next();

               if (++bad > 1)
               {
                   break;
                   std::cout << "Sniffer process failed! rc = " << rc << std::endl;
               }
            }
            else
            {
                bad = 0;
                if (sniffer_.IsComplete())
                {

                    auto pack = sniffer_.GetPacket();

                    int count = pack->Size();
                    const unsigned char* data = (const unsigned char*)pack->Pyload();
                    auto hdr = pack->Header();

                    std::cout << "Recieved fragmen data succefully." << std::endl;

                    std::cout << "IP-Address source: " << ::ntohl(hdr->src) << std::endl;
                    std::cout << "IP-Address destination: " << ::ntohl(hdr->dest) << std::endl;
                    std::cout << "Length (last packet): " << ::ntohs(hdr->length) << std::endl;
                    std::cout << "Packet-id: " << ::ntohs(hdr->id) << std::endl;

                    std::cout << "Total packet size: " << count <<  std::endl;

                    std::cout << "Data dump: " << std::endl;

                    for (int i = 0; i < count; i++)
                    {
                        std::cout << (int)data[i] << ", ";
                    }

                    std::cout << std::endl;

                    sniffer_.Next();
                }

                if (sniffer_.IsBad())
                {
                    std::cout << "ERROR!!! Bad packet!!" << std::endl;
                    sniffer_.Next();
                }

                p += rc;
                len -=rc;
            }

        }

    }
}
