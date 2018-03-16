#ifndef _OL_DATA_PACKET_H
#define _OL_DATA_PACKET_H

#include <memory>
#include <vector>

#include "OsnovaLab/Data/DataTypes.h"

namespace OsnovaLab
{
	namespace Data
	{
        using packet_data_t = char;
        using packet_data_ptr_t = packet_data_t*;

        class Packet
        {

            std::unique_ptr<packet_data_t> data_;

            collection_size_t size_;
            collection_size_t offset_;

            static packet_data_ptr_t packet_alloc(const packet_data_ptr_t pattern, const collection_size_t size);

        public:
            //конструкторы-деструкторы

            Packet();
            Packet(const packet_data_ptr_t data, const collection_size_t size);
            Packet(const Packet& packet);
            Packet(Packet&& packet);
            ~Packet();

            //перегрузка операторов
            Packet& operator=(const Packet& packet);
            Packet& operator=(Packet&& packet);
            Packet& operator+=(const Packet& packet);
            Packet& operator+=(Packet&& packet);


            //методы-модификаторы
            collection_size_t Pop(packet_data_ptr_t data, collection_size_t size);
            collection_size_t Push(const packet_data_ptr_t data, collection_size_t size);
            void Reset();
            /*template<typename T>
            inline auto Cast() const -> T* { return static_cast<T*>(data_.get()); }*/

            //методы доступа
            operator const packet_data_ptr_t() const;
            virtual const packet_data_ptr_t Data() const;
            virtual const collection_size_t Size() const;
            std::shared_ptr<Packet> Clone();

        };

	}
}


#endif

