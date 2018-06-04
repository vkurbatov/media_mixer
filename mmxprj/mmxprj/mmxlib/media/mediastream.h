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

            static const int DEFAULT_JITTER_FLASHBACK = 60;
            static const int DEFAULT_JITTER_SIZE = 6;
            static const int DEFAULT_SAMPLE_FREQ = 8000;
            static const int DEFAULT_SAMPLE_WIDTH = 20;

            JitterBuffer    jitter_;

            unsigned short  pack_id_;
            int             ref_count_;

            unsigned int    address_;
            unsigned short  port_;

            int             jitter_flashback_;

            friend class MediaPool;
            MediaStream(unsigned int address = 0, unsigned short port = 0, int jitter_flashback = DEFAULT_JITTER_FLASHBACK);

        public:
            MediaStream(MediaStream&& mediastream);
            //MediaStream(unsigned int address = 0, unsigned short port = 0, int jitter_flashback = DEFAULT_JITTER_FLASHBACK);
            MediaStream& operator=(MediaStream&& mediastream);
            ~MediaStream();

            int PutSample(const mmx::headers::MEDIA_DATA& media);
            const headers::MEDIA_SAMPLE* GetMediaSample() const;
            void Clear();

        };
    }
}

#endif
