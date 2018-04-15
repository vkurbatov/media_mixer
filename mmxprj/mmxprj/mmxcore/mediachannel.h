#ifndef _MMXMUX_MEDIA_CHANNEL_H
#define _MMXMUX_MEDIA_CHANNEL_H

#include "mediapool.h"

#include "mmxlib/headers/si.h"
#include "mmxlib/headers/media.h"
#include "mmxlib/headers/order645_2.h"

namespace mmxmux
{
    class MediaChannelPool;
    class MediaChannel
    {
        mmx::headers::SANGOMA_SORM_INFO sorm_;

        mmx::headers::ORM_INFO_HEADER orm_header_;

        const MediaStream* stream_a_;
        const MediaStream* stream_b_;

        MediaPool* media_pool_;


        friend class MediaChannelPool;
    public:

        MediaChannel();
        ~MediaChannel();
        MediaChannel(MediaChannel&& channel);
        MediaChannel& operator=(MediaChannel&& channel);
        int SetProxy(const mmx::headers::SANGOMA_PROXY_INFO* proxy = nullptr);
        const mmx::headers::SANGOMA_SORM_INFO& GetOrmInfo() const;
        int OrmInfoPack(void* data, int size);
        //const mmx::headers::ORM_INFO_PACKET& GetOrmInfo();
        void Drop();

    private:
        void setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm);


    };
}

#endif
