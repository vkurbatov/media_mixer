#ifndef _MMX_STAFF_PACKET_H
#define _MMX_STAFF_PACKET_H

#include <vector>

namespace mmx
{
    namespace staff
    {
        using packet_data_t = char;

        class Packet
        {
            std::vector<packet_data_t> data_;
            int idx_;

        public:

            // кострукторы

            Packet(int size = 0);
            Packet(const packet_data_t* data, int size);
            Packet(const Packet& packet);
            Packet(Packet&& packet);

            // модификаторы данных

            Packet& operator=(const Packet& packet);
            Packet& operator=(Packet&& packet);
            int Append(const Packet& packet);
            int Append(const packet_data_t* data, int size);
            Packet& operator += (const Packet& packet);
            friend Packet operator + (const Packet& packet1, const Packet& packet2);

            // модификатор доступа

            void Seek(int idx = 0);
            void Reset();

            //

            void Resize(int size);

            // индексаторы, методы доступа и кастомизаторы

            packet_data_t& operator[] (int idx);
            operator packet_data_t* ();
            packet_data_t* Data(int idx = 0);

            template<typename T>
            T* Cast(int idx = 0)
            {
                return (T*)Data(idx);
            }

            template<typename T>
            const T* Cast(int idx = 0) const
            {
                return (const T*)Data(idx);
            }

            const packet_data_t* Data(int idx = 0) const;
            int Size() const;
            int Position() const;
        };
    }
}

#endif
