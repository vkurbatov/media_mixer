#include "ippacketpool.h"

#include <algorithm>
#include "tools/timer.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("IPPacketPool", msg)

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
            DLOGT(LOG_BEGIN("IPPacketPool(%d, %d, %d)"), max_free_queue_size, min_garbage_size, garbage_time_life);
        }

        IPPacketPool::~IPPacketPool()
        {
            DLOGT(LOG_BEGIN("~IPPacketPool()"));
        }

        IPPacket* IPPacketPool::GetPacket(unsigned int src_address, unsigned short pack_id)
        {

            IPPacket* rc = nullptr;

            std::uint64_t id = getKey(src_address, pack_id);

            DLOGT(LOG_BEGIN("GetPacket(%d, %d)"), src_address, pack_id);

            // для начала проверим, не пора ли почистить мусор

            if (min_garbage_size_ > 0)
            {
                if (pool_.size() >= min_garbage_size_)
                {
                    DLOGT(LOG_BEGIN("GetPacket(): auto garbage cleaning"));
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

                    DLOGT(LOG_BEGIN("GetPacket(): packet get from q_free (%d)"), q_free_.size());
                    // pool_[id] = std::move(q_free_.front());
                    auto&& pack = q_free_.front();

                    pool_.insert(std::move(std::pair<std::uint64_t, IPPacket>(std::move(id), std::move(pack))));
                    q_free_.pop();

                }
                else
                {
                    DLOGT(LOG_BEGIN("GetPacket(): new instance packet"));
                    pool_.insert(std::move(std::pair<std::uint64_t, IPPacket>(std::move(id), std::move(IPPacket(src_address, pack_id)))));
                }

                it = pool_.find(id);

                // rc = &pool_[id];

            }
            else
            {
                DLOGT(LOG_BEGIN("GetPacket(): packet found in pool"));
            }

            if (it != pool_.end())
            {
                rc = &it->second;
                // на случай изъятия из списка свободных
                rc->pack_id_ = pack_id;
                rc->src_addr_ = src_address;
            }

            return rc;

        }

        bool IPPacketPool::Release(IPPacket* packet)
        {

            bool rc = false;

            DLOGT(LOG_BEGIN("Release(%x)"), DLOG_POINTER(packet));

            if (packet != nullptr)
            {

                std::uint64_t id = getKey(packet->src_addr_, packet->pack_id_);

                auto it = pool_.find(id);

                // элемент найден в пуле

                if (it != pool_.end())
                {

                    DLOGT(LOG_BEGIN("Release(): Packet found in pool"));

                    // сбрасываем пакет

                    it->second.Reset();

                    // если разрешили кэш свободных

                    if (max_free_queue_size_ != 0)
                    {

                        DLOGT(LOG_BEGIN("Release(): Packet put in q_free"));

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
                else
                {
                    DLOGW(LOG_BEGIN("Release(%x): Packet not found in pool"), DLOG_POINTER(packet));
                }

            }
            else
            {
                DLOGE(LOG_BEGIN("Release(%x): invalid argument"), DLOG_POINTER(packet));
            }

            return rc;

        }

        // очистка "мусора" (несобранных покетов) по времени жизни (-1 удалит все)

        int IPPacketPool::ClearGarbage(int time_life)
        {
            int rc = 0;

            rm_list_.clear();         

            DLOGT(LOG_BEGIN("ClearGarbage(%d)"), time_life);

            for (auto it = pool_.begin(); it != pool_.end(); it++)
            {

                int elapsed = tools::Timer::GetTicks() - it->second.t_stamp_;

                if (elapsed > time_life)                 
                {
                    rm_list_.push_back(&it->second);
                }

            }

            DLOGT(LOG_BEGIN("ClearGarbage(): find %d garbage packet"), rm_list_.size());

            for (auto& p : rm_list_)
            {

                DLOGD(LOG_BEGIN("ClearGarbage(%d): remove packet %x"), DLOG_POINTER(p));

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

            DLOGD(LOG_BEGIN("Reset(): pool_: %d packets"), pool_.size());

            while (!q_free_.empty())
            {
                q_free_.pop();
            }

            pool_.clear();
        }
    }
}
