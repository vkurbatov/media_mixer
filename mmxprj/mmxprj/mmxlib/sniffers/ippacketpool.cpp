#include "ippacketpool.h"

#include <algorithm>

namespace mmx
{
    namespace sniffers
    {
        IPPacketPool::IPPacketPool(int max_free_queue_size,
                                   int min_garbage_size,
                                   int garbage_time_life) :
            max_free_queue_size_(max_free_queue_size),
            min_garbage_size_(min_garbage_size),
            garbage_time_life_(garbage_time_life)
        {

        }

        IPPacket* IPPacketPool::GetPacket(unsigned int src_address, unsigned short pack_id)
        {

            IPPacket* rc = nullptr;

            std::uint64_t id = getKey(src_address, pack_id);

            // для начала проверим, не пора ли почистить мусор

            if (min_garbage_size_ > 0)
            {
                if (pool_.size() >= min_garbage_size_)
                {
                    ClearGarbage(garbage_time_life_);
                }
            }

            auto it = pool_.find(id);

            // элемента нет в map

            if (it == pool_.end())
            {
                // проверим очередь свободных

                if (!q_free_.empty())
                {

                    pool_[id] = std::move(q_free_.front());
                    q_free_.pop();

                }

                rc = &pool_[id];

            }
            else
            {
                rc = &it->second;
            }


            if (rc != nullptr)
            {
                rc->pack_id_ = pack_id;
                rc->src_addr_ = src_address;
            }

            return rc;

        }

        bool IPPacketPool::Release(IPPacket* packet)
        {

            bool rc = false;

            if (packet != nullptr)
            {

                std::uint64_t id = getKey(packet->src_addr_, packet->pack_id_);

                auto it = pool_.find(id);

                // элемент найден в пуле

                if (it != pool_.end())
                {

                    // сбрасываем пакет

                    it->second.Reset();

                    // если разрешили кэш свободных

                    if (max_free_queue_size_ != 0)
                    {

                        // отправляем пакет в список свободных

                        q_free_.push(std::move(it->second));

                    }

                    // отрицательное значение символизирует о бесконечном кэше свободных

                    else if (max_free_queue_size_ > 0)
                    {
                        while (q_free_.size() > max_free_queue_size_)
                        {
                            q_free_.pop();
                        }
                    }

                    // из пула элемент удалим

                    pool_.erase(it);

                    rc = true;
                }

            }

            return rc;

        }

        // очистка "мусора" (несобранных покетов) по времени жизни (-1 удалит все)

        int IPPacketPool::ClearGarbage(int time_life)
        {
            int rc = 0;

            rm_list_.clear();         

            for (auto it = pool_.begin(); it != pool_.end(); it++)
            {

                int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - it->second.t_stamp_).count();

                if (elapsed > time_life)
                {
                    rm_list_.push_back(&it->second);
                }

            }

            for (auto& p : rm_list_)
            {
                Release(p);
            }

            rm_list_.clear();

            return rc;
        }

        int IPPacketPool::Count() const
        {
            return pool_.size();
        }

        std::uint64_t IPPacketPool::getKey(unsigned int src_address, unsigned short pack_id)
        {
            return (std::uint64_t)src_address << 16 | (std::uint64_t)pack_id;
        }

        void IPPacketPool::Reset()
        {
            while (!q_free_.empty())
            {
                q_free_.pop();
            }

            pool_.clear();
        }
    }
}
