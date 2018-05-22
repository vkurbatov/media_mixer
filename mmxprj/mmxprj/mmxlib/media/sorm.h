#ifndef _MMX_MEDIA_SORM_H
#define _MMX_MEDIA_SORM_H

#include "mediapool.h"

#include "headers/si.h"
#include "headers/media.h"
#include "headers/order645_2.h"
#include "data/idpwriter.h"

namespace mmx
{
    namespace media
    {

        class SormPool;
        class Sorm
        {
            const static int STREAM_COUNT = 2;
            const static int DEFAULT_MIXER_GAIN = 70;

            mmx::headers::SANGOMA_SORM_INFO sorm_info_;

            mmx::headers::ORDER_645_2_HEADER order_header_;

            const MediaStream* streams_[STREAM_COUNT];

            unsigned int        rtp_ssrcs_[STREAM_COUNT];
            unsigned int        rtp_pack_ids_[STREAM_COUNT];

            unsigned char       mixer_gain_;

            MediaPool&          media_pool_;



            friend class SormPool;
        public:

            Sorm(MediaPool& media_pool, unsigned char mixer_gain = DEFAULT_MIXER_GAIN);
            ~Sorm();
            Sorm(Sorm&& channel);
            Sorm& operator=(Sorm&& channel);
            int SetProxy(const mmx::headers::SANGOMA_PROXY_INFO* proxy = nullptr);
            const mmx::headers::SANGOMA_SORM_INFO& GetOrmInfo() const;
            int OrmInfoPack(data::IDataPacketWriter& writer);
            void Drop();

        private:
            void setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm);


        };
    }
}

#endif
