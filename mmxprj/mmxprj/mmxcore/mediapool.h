#ifndef _MMXMUX_MEDIA_STREAM_POOL_H
#define _MMXMUX_MEDIA_STREAM_POOL_H

#include "mediastream.h"

#include <unordered_map>
#include <queue>
#include <vector>
#include <typeinfo>

namespace mmxmux
{

    class MediaPool
    {
        std::unordered_map<std::uint64_t, MediaStream> pool_;
        std::queue<MediaStream> q_free_;
        std::vector<MediaStream*> rm_list_;

        int max_free_queue_size_;
        int min_garbage_size_;
        int garbage_time_life_;

    public:
        MediaPool(int max_free_queue_size = -1,
                     int min_garbage_size = 0,
                     int garbage_time_life = 0);

        MediaStream* GetStream(unsigned int address, unsigned short port);
        MediaStream* FindStream(unsigned int address, unsigned short port);
        bool Release(const MediaStream* stream, int count = 1);
        bool Release(unsigned int address, unsigned short port, int count = 1);
        int ClearGarbage(int time_life = 0);
        int Count() const;
        void Reset();
    private:
        std::uint64_t getKey(unsigned int address, unsigned short port);
    };

}

#endif
