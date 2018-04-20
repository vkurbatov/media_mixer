#include "mediapool.h"

#include <chrono>

#include <sys/time.h>

namespace mmx
{
    namespace media
    {
        MediaPool::MediaPool(int max_free_queue_size,
                                   int min_garbage_size,
                                   int garbage_time_life) :
            max_free_queue_size_(max_free_queue_size),
            min_garbage_size_(min_garbage_size),
            garbage_time_life_(garbage_time_life)
        {

        }

        MediaStream* MediaPool::GetStream(unsigned int address, unsigned short port)
        {

            MediaStream* rc = nullptr;

            std::uint64_t id = getKey(address, port);

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
                rc->ref_count_++;
                rc->port_ = port;
                rc->address_ = address;
            }

            return rc;

        }

        MediaStream* MediaPool::FindStream(unsigned int address, unsigned short port)
        {
            MediaStream* rc = nullptr;

            auto it = pool_.find(getKey(address, port));

            if (it != pool_.end())
            {
                rc = &it->second;
            }

            return rc;
        }

        bool MediaPool::Release(const MediaStream* stream, int count)
        {

            bool rc = false;

            if (stream != nullptr)
            {

                rc = Release(stream->address_, stream->port_, count);

            }

            return rc;

        }

        bool MediaPool::Release(unsigned int address, unsigned short port, int count)
        {
            bool rc = false;

                std::uint64_t id = getKey(address, port);

                auto it = pool_.find(id);

                // элемент найден в пуле

                if (it != pool_.end())
                {

                    // уменьшаем счетчик ссылок (-1 - удалить все ссылки)

                if (it->second.ref_count_ < count || count < 0)
                {
                    it->second.ref_count_ = 0;
                }
                else
                {
                    it->second.ref_count_ -= count;
                }

                // если ссылок больше нет, то удаляем стрим

                if (it->second.ref_count_ == 0)
                {

                    // сбрасываем стрим

                    it->second.Clear();

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

                }

                rc = true;
            }

            return rc;
        }

        // очистка "мусора" (устаревший медиапотоков) по времени жизни (-1 удалит все)

        int MediaPool::ClearGarbage(int time_life)
        {
            int rc = 0;

            rm_list_.clear();

            ::timeval tv;

            ::gettimeofday(&tv,0);


            unsigned int timestamp = (tv.tv_sec % 86400) * 1000 + (tv.tv_usec / 1000);

            for (auto it = pool_.begin(); it != pool_.end(); it++)
            {

                auto smpl = it->second.GetSample();

                if (smpl == nullptr || (int)((timestamp - smpl->header.timestamp) % 86400000) > time_life)
                {
                    rm_list_.push_back(&it->second);
                }

            }

            for (auto& p : rm_list_)
            {
                Release(p, -1);
            }

            rm_list_.clear();

            return rc;
        }

        int MediaPool::Count() const
        {
            return pool_.size();
        }

        std::uint64_t MediaPool::getKey(unsigned int src_address, unsigned short pack_id)
        {
            return (std::uint64_t)src_address << 16 | (std::uint64_t)pack_id;
        }

        void MediaPool::Reset()
        {
            while (!q_free_.empty())
            {
                q_free_.pop();
            }

            pool_.clear();
        }
    }
}
