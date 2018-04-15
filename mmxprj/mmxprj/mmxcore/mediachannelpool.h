#ifndef _MMXMUX_MEDIA_CHANNEL_POOL_H
#define _MMXMUX_MEDIA_CHANNEL_POOL_H

#include "mediachannel.h"
#include "mediapool.h"

#include <unordered_map>
#include <list>
#include <queue>
#include <vector>
#include <typeinfo>

namespace mmxmux
{

    class MediaChannelPool
    {
        std::unordered_map<std::uint64_t, MediaChannel> pool_;
        //std::list<MediaChannel> pool_;
        std::queue<MediaChannel> q_free_;
        std::vector<MediaChannel*> rm_list_;

        MediaPool& media_pool_;

        int max_free_queue_size_;

    public:

        MediaChannelPool(MediaPool& media_pool, int max_free_queue_size = -1);

        MediaChannel* GetChannel(const mmx::headers::SANGOMA_SORM_INFO& sorm, const mmx::headers::SANGOMA_PROXY_INFO& proxy);
        bool Release(MediaChannel* channel);
        int Count() const;
        void Reset();
    private:
        std::uint64_t getKey(const mmx::headers::SANGOMA_SORM_INFO& sorm);
    };

}

#endif
