#include "sormpool.h"

#include <chrono>

#include <sys/time.h>

namespace mmx
{
    namespace media
    {
        SormPool::SormPool(MediaPool& media_pool, int max_free_queue_size) :
            media_pool_(media_pool),
            max_free_queue_size_(max_free_queue_size)
        {

        }

        Sorm* SormPool::GetChannel(const mmx::headers::SANGOMA_SORM_INFO& sorm, const mmx::headers::SANGOMA_PROXY_INFO& proxy)
        {

            Sorm* rc = nullptr;

            std::uint64_t id = getKey(sorm);


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
                rc->setSorm(sorm);
                rc->media_pool_ = &media_pool_;
                rc->SetProxy(&proxy);
            }

            return rc;

        }

        bool SormPool::Release(Sorm* channel)
        {

            bool rc = false;

            if (channel != nullptr)
            {

                rc = Release(channel->sorm_);

            }

            return rc;

        }

        bool SormPool::Release(const mmx::headers::SANGOMA_SORM_INFO& sorm)
        {
            bool rc = false;

            std::uint64_t id = getKey(sorm);

            auto it = pool_.find(id);

            // элемент найден в пуле

            if (it != pool_.end())
            {

                    // дропаем собраный пакет

                    it->second.Drop();

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

            return rc;
        }

        int SormPool::Count() const
        {
            return pool_.size();
        }

        std::vector<Sorm*> SormPool::GetChannels()
        {
            channel_list_.clear();

            for (auto& c : pool_)
            {
                channel_list_.push_back(&c.second);
            }

            return channel_list_;
        }

        std::uint64_t SormPool::getKey(const mmx::headers::SANGOMA_SORM_INFO& sorm)
        {
            return (std::uint64_t)sorm.call_id
                    | (std::uint64_t)sorm.object_id << 16
                    | (std::uint64_t)sorm.sorm_id << 32;
        }

        void SormPool::Reset()
        {
            while (!q_free_.empty())
            {
                q_free_.pop();
            }

            pool_.clear();
        }
    }
}
