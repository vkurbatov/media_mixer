#ifndef _MMX_STAFF_MEDIA_PACKET_H
#define _MMX_STAFF_MEDIA_PACKET_H

#include "packet.h"
#include "headers/media.h"
#include "headers/datapack.h"



namespace mmx
{
    namespace staff
    {
        static const int MAX_PACK_SIZE = 0xFFFF;

        class DataPacket
        {
            Packet packet_;
            int offset_;

        public:
            DataPacket(Packet&& packet);
            DataPacket(const Packet& packet);
            DataPacket(int size);
            ~DataPacket();

            headers::PDATA_PACK Init(int pack_id);

            bool IsValid() const;

            operator headers::PDATA_PACK();

            headers::PDATA_PACK Data();

            const headers::DATA_PACK* Data() const;

            headers::PDATA_BLOCK QueryData(int size);

            headers::PDATA_BLOCK Next();

            void Reset();

            int Count() const;

            int Size() const;

            bool IsEmpty() const;

        };
    }
}

#endif
