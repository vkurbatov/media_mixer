#ifndef _MMX_MEDIA_JITTER_H
#define _MMX_MEDIA_JITTER_H

#include "headers/media.h"

namespace mmx
{
    namespace media
    {
        class JitterBuffer
        {
            int         width_;
        public:
            JitterBuffer(int width =  160);
            ~JitterBuffer();

            int PutSample(const mmx::headers::MEDIA_DATA& media);
            const mmx::headers::MEDIA_SAMPLE* GetSample() const;
            void Clear();
        };
    }
}

#endif
