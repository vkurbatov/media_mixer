#ifndef _MMX_MEDIA_SORM_POOL_H
#define _MMX_MEDIA_SORM_POOL_H

#include "sorm.h"
#include "mediapool.h"

#include <unordered_map>
#include <list>
#include <queue>
#include <vector>
#include <typeinfo>

namespace mmx
{
    namespace media
    {
        class SormPool
        {
            std::unordered_map<std::uint64_t, Sorm, std::hash<unsigned short>> pool_;
            //std::list<Sorm> pool_;
            std::queue<Sorm> q_free_;
            std::vector<Sorm*> rm_list_;
            std::vector<Sorm*> channel_list_;

            MediaPool& media_pool_;

            int max_free_queue_size_;

        public:

            SormPool(MediaPool& media_pool, int max_free_queue_size = -1);

            Sorm* GetChannel(const mmx::headers::SANGOMA_SORM_INFO& sorm_info, const mmx::headers::SANGOMA_PROXY_INFO& proxy);
            Sorm* FindChannel(const mmx::headers::SANGOMA_SORM_INFO& sorm_info);
            bool Release(Sorm* channel);
            bool Release(const mmx::headers::SANGOMA_SORM_INFO& sorm_info);
            int Count() const;
            void Reset();

            std::vector<Sorm*> GetChannels();


        private:
            std::uint64_t getKey(const mmx::headers::SANGOMA_SORM_INFO& sorm_info);
        };
    }

}

#endif
