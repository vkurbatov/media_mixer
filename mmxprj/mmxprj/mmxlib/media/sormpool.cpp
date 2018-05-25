#include "sormpool.h"

#include <chrono>

#include <sys/time.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("SormPool", msg)

namespace mmx
{
    namespace media
    {
        SormPool::SormPool(MediaPool& media_pool, int max_free_queue_size, int mixed_gain_sorm) :
            media_pool_(media_pool),
            max_free_queue_size_(max_free_queue_size),
            mixed_gain_sorm_(mixed_gain_sorm)
        {
            DLOGT(LOG_BEGIN("SormPool(&%x, %d)"), DLOG_POINTER(&media_pool), max_free_queue_size);
        }

        SormPool::SormPool(SormPool &&sormpool) :
            media_pool_(sormpool.media_pool_),
            max_free_queue_size_(sormpool.max_free_queue_size_),
            mixed_gain_sorm_(sormpool.mixed_gain_sorm_),
            q_free_(std::move(sormpool.q_free_)),
            rm_list_(std::move(rm_list_)),
            channel_list_(std::move(channel_list_))
        {

        }

        Sorm* SormPool::GetSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm_info, const mmx::headers::SANGOMA_PROXY_INFO& proxy)
        {

            Sorm* rc = nullptr;

            std::uint64_t id = getKey(sorm_info);


            DLOGT(LOG_BEGIN("GetChannel(&%x, &%x): id = %d"), DLOG_POINTER(&sorm_info), DLOG_POINTER(&proxy), id);

            auto it = pool_.find(id);

            // элемента нет в map

            if (it == pool_.end())
            {

                DLOGT(LOG_BEGIN("GetChannel(): sorm not found in pool"));
                // проверим очередь свободных

                if (!q_free_.empty())
                {
                    DLOGT(LOG_BEGIN("GetChannel(): sorm get from queue of free"));

                    pool_.insert(std::move(std::pair<std::uint64_t, Sorm>(std::move(id), std::move(q_free_.front()))));

                    q_free_.pop();

                }
                else
                {

                    DLOGT(LOG_BEGIN("GetChannel(): sorm get from new instance"));

                    pool_.insert(std::move(std::pair<std::uint64_t, Sorm>(std::move(id), std::move(Sorm(media_pool_)))));
                }

                it = pool_.find(id);
                channel_list_.clear();

            }

            if (it != pool_.end())
            {
                DLOGT(LOG_BEGIN("GetChannel(): sorm = %x"), DLOG_POINTER(rc));
                rc = &it->second;
                rc->setSorm(sorm_info);
                rc->SetProxy(&proxy);
                rc->mixer_gain_ = mixed_gain_sorm_;
            }
            else
            {
                DLOGE(LOG_BEGIN("GetChannel(): Don't create sorm object!"));
                errno = ENOMEM;
            }

            return rc;

        }

        Sorm *SormPool::FindSorm(const headers::SANGOMA_SORM_INFO &sorm_info)
        {
            Sorm* rc = nullptr;

            auto it = pool_.find(getKey(sorm_info));

            if (it != pool_.end())
            {
                rc = &it->second;
                DLOGT(LOG_BEGIN("FindChannel(%x): found in pool"), DLOG_POINTER(&sorm_info));
            }
            else
            {
                DLOGT(LOG_BEGIN("FindChannel(%x): not found in pool"), DLOG_POINTER(&sorm_info));
            }

            return rc;
        }

        bool SormPool::Release(Sorm* sorm)
        {

            bool rc = false;

            DLOGT(LOG_BEGIN("Release(%x, %d)"), DLOG_POINTER(sorm));

            if (sorm != nullptr)
            {

                rc = Release(sorm->sorm_info_);

            }

            return rc;

        }

        bool SormPool::Release(const mmx::headers::SANGOMA_SORM_INFO& sorm_info)
        {
            bool rc = false;

            std::uint64_t id = getKey(sorm_info);

            DLOGT(LOG_BEGIN("Release(%x, %d): key = %d"), DLOG_POINTER(&sorm_info), id );

            auto it = pool_.find(id);

            // элемент найден в пуле

            if (it != pool_.end())
            {

                DLOGT(LOG_BEGIN("Release(): sorm found %x"), DLOG_POINTER(&it->second));

                // дропаем собраный пакет

                it->second.Drop();

                // если разрешили кэш свободных

                if (max_free_queue_size_ != 0)
                {

                    DLOGT(LOG_BEGIN("Release(): sorm push in queue of free"));

                    // отправляем пакет в список свободных

                    q_free_.push(std::move(it->second));

                }

                // отрицательное значение символизирует о бесконечном кэше свободных

                else if (max_free_queue_size_ > 0)
                {
                    while (q_free_.size() > max_free_queue_size_)
                    {
                        DLOGT(LOG_BEGIN("Release(): remove sorm %x from queue of free"), &q_free_.front());
                        q_free_.pop();
                    }
                }

                // из пула элемент удалим

                DLOGT(LOG_BEGIN("Release(): remove sorm %x from pool"), &q_free_.front());
                pool_.erase(it);

                rc = true;
                channel_list_.clear();
            }

            return rc;
        }

        int SormPool::Count() const
        {
            return pool_.size();
        }

        std::vector<Sorm*> SormPool::GetSorms()
        {

            if (channel_list_.empty())
            {
                for (auto& c : pool_)
                {
                    channel_list_.push_back(&c.second);
                }
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
            channel_list_.clear();
        }
    }
}
