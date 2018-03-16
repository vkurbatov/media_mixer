#include "OsnovaLab/Tools/Net/PacketQueue.h"

namespace OsnovaLab
{
    namespace Tools
    {
        namespace Net
        {
            PacketQueue::PacketQueue() : queue_(), total_size_(0)
            {

            }

            PacketQueue::~PacketQueue()
            {
                queue_.Clear();
            }

            Data::collection_size_t PacketQueue::Count() const
            {
                return queue_.Count();
            }

            Data::collection_size_t PacketQueue::TotalSize() const
            {
                return total_size_;
            }

            // append - добавить блок данных к существующему пакету
            Data::collection_size_t PacketQueue::Push(Data::packet_data_ptr_t data, Data::collection_size_t size, bool append)
            {
                Data::collection_size_t rc = -1;

                packet_t* pack;

                if (append == true && (pack = queue_.Front()) != nullptr)
                {
                    rc = pack->Push(data, size);
                }
                else
                {
                    rc = queue_.Push(Data::Packet(data, size));
                }

                if (rc > 0)
                {
                    total_size_ += rc;
                }

                return rc;
            }

            //read_all - вычитывать в буффер максимальнро возможное количество данных из всех пакетов
            Data::collection_size_t PacketQueue::Pop(Data::packet_data_ptr_t data, Data::collection_size_t size, bool read_all)
            {
                Data::collection_size_t rc = -1;

                Data::collection_size_t total = 0;

                bool loop;

                do
                {
                    auto pack = queue_.Front();

                    loop = false;

                    if (pack != nullptr)
                    {
                        auto rd = pack->Pop(data == nullptr ? nullptr : data + total, size);

                        if (rd > 0)
                        {
                            total += rd;
                            size -= rd;

                            rc = total;

                            if (pack->Size() == 0)
                            {
                                queue_.Pop(nullptr);
                            }

                            loop = read_all && size > 0;
                        }

                    }

                }while(loop == true);

                if (rc > 0)
                {
                    total_size_ -= rc;
                }

                return rc;
            }

            packet_t* PacketQueue::Front()
            {
                return queue_.Front();
            }
        }
    }
}
