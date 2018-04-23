#ifndef _MMXMUX_MEDIA_STREAM_H
#define _MMXMUX_MEDIA_STREAM_H

#include <typeinfo>
#include <vector>

#include "headers/media.h"
#include "sample.h"
#include "jitterbuffer.h"

namespace mmx
{
    namespace media
    {
        class MediaPool;
        class MediaStream
        {

            JitterBuffer    jitter_;

            unsigned short pack_id_;
            int ref_count_;

            unsigned int address_;
            unsigned short port_;


            friend class MediaPool;

        public:

            MediaStream(unsigned int address = 0, unsigned short port = 0);
            MediaStream(MediaStream&& mediastream);
            MediaStream& operator=(MediaStream&& mediastream);

            int PutSample(const mmx::headers::MEDIA_DATA& media);
            const headers::MEDIA_SAMPLE* GetSample() const;
            void Clear();

        };
    }
}

#endif
