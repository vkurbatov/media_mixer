#ifndef _OL_DATA_PACKET_H
#define _OL_DATA_PACKET_H

#include <memory>

#include "OsnovaLab/Data/DataTypes.h"

namespace OsnovaLab
{
	namespace Data
	{
        using packet_data_t = unsigned char;
        using packet_data_ptr_t = packet_data_t*;

        class Packet
        {

           std::unique_ptr<packet_data_t> data_;
           collection_size_t size_;

        public:
            Packet();
            Packet(const raw_data_ptr_t data, const collection_size_t size);
            Packet(const Packet& packet);
            Packet(Packet&& packet);
            ~Packet();

            Packet& operator=(const Packet& packet);
            Packet& operator=(Packet&& packet);

            /*template<typename T>
            inline auto Cast() const -> T* { return static_cast<T*>(data_.get()); }*/

            operator const packet_data_ptr_t() const;
            operator const raw_data_ptr_t() const;
            const packet_data_ptr_t Data() const;
            const collection_size_t Size() const;
            Packet* Clone();
        };

	}
}


#endif

