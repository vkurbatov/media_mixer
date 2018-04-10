#include "defines.h"
#include <iostream>

#include <cstring>

#include "mmxlib/sniffers/dpsniffer.h"
#include "mmxlib/data/dpwriter.h"
#include "mmxlib/data/dpreader.h"

#include <netdb.h>

#define BLOCK_COUNT 4
#define TEST_DATA_SIZE 16

namespace mmxtest
{
    int test6()
    {
        union
        {
            mmx::headers::DATA_PACK packet1;
            char raw1[1600];
        };

        union
        {
            mmx::headers::DATA_PACK packet2;
            char raw2[1600];
        };

        union
        {
            mmx::headers::DATA_PACK packet3;
            char raw3[1600];
        };

        union
        {
            mmx::headers::DATA_PACK packet4;
            char raw4[1600];
        };

        union
        {
            mmx::headers::DATA_PACK packet5;
            char raw5[1600];
        };

        mmx::headers::PDATA_PACK packets[] =
        {
            &packet1,
            &packet2,
            &packet3,
            &packet4,
            &packet5
        };

        std::memset(raw1, 0, sizeof(raw1));
        std::memset(raw2, 0, sizeof(raw2));
        std::memset(raw3, 0, sizeof(raw3));
        std::memset(raw4, 0, sizeof(raw4));
        std::memset(raw5, 0, sizeof(raw5));

        mmx::data::DataPacketWriter writer[5] =
        {
            mmx::data::DataPacketWriter(raw1, sizeof(raw1)),
            mmx::data::DataPacketWriter(raw2, sizeof(raw2)),
            mmx::data::DataPacketWriter(raw3, sizeof(raw3)),
            mmx::data::DataPacketWriter(raw4, sizeof(raw4)),
            mmx::data::DataPacketWriter(raw5, sizeof(raw5)),
        };

        for (int j = 0; j < 5; j++)
        {
            writer[j].BuildPacket(j * 10);
        }

        for (int i = 0; i < BLOCK_COUNT; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                auto p = writer[j].QueryBlock(TEST_DATA_SIZE);
                if (p != nullptr)
                {

                    for (int k = 0; k < p->header.length - sizeof(p->header); k++)
                    {
                        p->data[k] = i * 50 + j * 10 + k;
                    }

                writer[j].Commit();

                }
            }
        }


        mmx::sniffers::DataPackSniffer sniffer_;

        char buffer[10000];
        int off = 0;
        char* p = buffer + off;
        int rc = off;

        for (auto pack : packets)
        {

            std::memcpy(p, pack, pack->header.length);
            p += pack->header.length + 4;
            rc += pack->header.length + 4;

        }


        int len = rc;
        int part_size = 1;
        int bad = 0;

        p = buffer;

        while (len > 0)
        {
            part_size = std::min(len, part_size);

            rc = sniffer_.PutStream(p, part_size);

            if (rc < 0)
            {
               std::cout << "Error fragmen sniffer. rc = " << rc << std::endl;

               sniffer_.Drop();

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

                    auto pack = sniffer_.GetDataPacket();

                    if (pack != nullptr)
                    {
                        mmx::data::DataPacketReader reader((mmx::headers::DATA_PACK_HEADER*)pack);

                        std::cout << "Get data pack succefully." << std::endl;

                        std::cout << "Magic: " << pack->header.magic << std::endl;
                        std::cout << "Block count: " << pack->header.block_count << std::endl;
                        std::cout << "Length: " << pack->header.length << std::endl;
                        std::cout << "Packet-id: " << pack->header.pack_id << std::endl;

                        auto p = reader.GetBlock();

                        while(p != nullptr)
                        {
                            std::cout << "Data dump of block #" << p->header.block_id << std::endl;

                            for (int i = 0; i < p->header.length - sizeof(p->header); i++)
                            {
                                if (i > 0)
                                {
                                    std::cout << ", ";
                                }
                                std::cout << (int)p->data[i];
                            }

                            std::cout << std::endl;

                            p = reader.GetBlock();
                        }

                    }

                    sniffer_.Drop();
                }

                if (sniffer_.IsBad())
                {
                    std::cout << "ERROR!!! Bad packet!!" << std::endl;
                    sniffer_.Drop();
                }

                p += rc;
                len -=rc;
            }

        }

    }
}
