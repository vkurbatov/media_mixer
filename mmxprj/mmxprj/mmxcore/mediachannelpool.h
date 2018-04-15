#ifndef _MMXMUX_MEDIA_STREAM_POOL_H
#define _MMXMUX_MEDIA_STREAM_POOL_H

#include "mediastream.h"

#include <unordered_map>
#include <queue>
#include <vector>
#include <typeinfo>

namespace mmxmux
{

    class MediaChannelPool
    {
        std::unordered_map<std::uint64_t, MediaStream> pool_;
        std::queue<MediaStream> q_free_;
        std::vector<MediaStream*> rm_list_;


    public:
        MediaChannelPool();

        MediaStream* GetChannel(unsigned char channel_id);
        bool Release(MediaStream* stream, int count = 1);
        int Count() const;
        void Reset();

    };

}

#endif
