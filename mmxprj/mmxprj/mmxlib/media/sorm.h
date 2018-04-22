#ifndef _MMX_MEDIA_SORM_H
#define _MMX_MEDIA_SORM_H

#include "mediapool.h"

#include "headers/si.h"
#include "headers/media.h"
#include "headers/order645_2.h"

namespace mmx
{
    namespace media
    {
        class SormPool;
        class Sorm
        {
            mmx::headers::SANGOMA_SORM_INFO sorm_;

            mmx::headers::ORM_INFO_HEADER orm_header_;

            const MediaStream* streams_[2];

            MediaPool* media_pool_;


            friend class SormPool;
        public:

            Sorm();
            ~Sorm();
            Sorm(Sorm&& channel);
            Sorm& operator=(Sorm&& channel);
            int SetProxy(const mmx::headers::SANGOMA_PROXY_INFO* proxy = nullptr);
            const mmx::headers::SANGOMA_SORM_INFO& GetOrmInfo() const;
            int OrmInfoPack(void* data, int size);
            //const mmx::headers::ORM_INFO_PACKET& GetOrmInfo();
            void Drop();

        private:
            void setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm);


        };
    }
}

#endif
