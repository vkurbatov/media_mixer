#include "mediapool.h"

#include <chrono>

#include <sys/time.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("MediaPool", msg)


namespace mmx
{
    namespace media
    {
        MediaPool::MediaPool(int max_free_queue_size,
                                   int jitter_flashback) :
            max_free_queue_size_(max_free_queue_size),
            jitter_flashback_(jitter_flashback)
        {
            DLOGT(LOG_BEGIN("MediaPool(%d, %d)"), max_free_queue_size, jitter_flashback);
        }

        MediaStream* MediaPool::GetStream(unsigned int address, unsigned short port)
        {

            MediaStream* rc = nullptr;

            std::uint64_t id = getKey(address, port);

            DLOGT(LOG_BEGIN("GetStream(%d, %d), key = %d"), address, port, id);


            auto it = pool_.find(id);


            // элемента нет в map

            if (it == pool_.end())
            {
                DLOGT(LOG_BEGIN("GetStream(): stream not found in pool"));

                if (!q_free_.empty())
                {

                    DLOGT(LOG_BEGIN("GetStream(): stream get from the queue (%d) of free"), q_free_.size());

                    pool_.insert(std::move(std::pair<std::uint64_t, MediaStream>(std::move(id), std::move(q_free_.front()))));

                    q_free_.pop();

                }
                else
                {

                    DLOGT(LOG_BEGIN("GetChannel(): sorm get from new instance"));

                    pool_.insert(std::move(std::pair<std::uint64_t, MediaStream>(std::move(id), std::move(MediaStream(address, port, jitter_flashback_)))));
                }

                it = pool_.find(id);

            }
            else
            {
                DLOGT(LOG_BEGIN("GetStream(): stream found in pool"));
            }

            if (it != pool_.end())
            {
                rc = &it->second;

                if (rc != nullptr)
                {
                    rc->ref_count_++;
                    rc->port_ = port;
                    rc->address_ = address;

                    DLOGT(LOG_BEGIN("GetStream(): get stream success: ref_count_ = %d"), rc->ref_count_);
                }
            }
            else
            {
                errno = ENOMEM;
                DLOGE(LOG_BEGIN("GetStream(%d, %d): get stream error"),address ,port, id);
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
                DLOGT(LOG_BEGIN("FindStream(%d, %d): found in pool"), address, port);
            }
            else
            {
                DLOGT(LOG_BEGIN("FindStream(%d, %d): not found in pool"), address, port);
            }

            return rc;
        }

        bool MediaPool::Release(const MediaStream* stream, int count)
        {

            bool rc = false;

            if (stream != nullptr)
            {

                rc = Release(stream->address_, stream->port_, count);

                DLOGT(LOG_BEGIN("Release(%x,%d): release result = %d"), &stream, count, rc);

            }
            else
            {
                DLOGT(LOG_BEGIN("Release(%x,%d): invalid argument"), &stream, count);
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

                DLOGT(LOG_BEGIN("Release(%d, %d, %d): key %d found stream %x"), address, port, count, id, DLOG_POINTER(&it->second));

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

                    DLOGT(LOG_BEGIN("Release(): sample %x, ref_count_ = 0"), DLOG_POINTER(&it->second));

                    // сбрасываем стрим

                    it->second.Clear();

                    // если разрешили кэш свободных

                    if (max_free_queue_size_ != 0)
                    {

                        // отправляем стрим в список свободных

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

        // очистка "мусора" (устаревших медиапотоков) по времени жизни (-1 удалит все)

        int MediaPool::ClearGarbage(int time_life)
        {

            int rc = 0;

            rm_list_.clear();

            ::timeval tv;

            ::gettimeofday(&tv,0);


            unsigned int timestamp = (tv.tv_sec % 86400) * 1000 + (tv.tv_usec / 1000);

            DLOGT(LOG_BEGIN("ClearGarbage(%d): ts = %d"), time_life, timestamp);


            for (auto it = pool_.begin(); it != pool_.end(); it++)
            {

                auto smpl = it->second.GetMediaSample();

                if (smpl == nullptr || (int)((timestamp - smpl->header.timestamp) % 86400000) > time_life)
                {
                    rm_list_.push_back(&it->second);
                }

            }

            DLOGT(LOG_BEGIN("ClearGarbage(): found for remove %d streams"), rc);

            for (auto& p : rm_list_)
            {
                Release(p, -1);
            }

            rc = rm_list_.size();

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
