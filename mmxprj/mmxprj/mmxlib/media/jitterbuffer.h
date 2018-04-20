#ifndef _MMX_MEDIA_JITTER_H
#define _MMX_MEDIA_JITTER_H

#include "sample.h"

#include <vector>

namespace mmx
{
    namespace media
    {
        class JitterBuffer
        {
            int                     jitter_;
            std::vector<Sample>     samples_;
            int                     current_;


        public:
            JitterBuffer(int sample_width_ =  160);
            JitterBuffer(JitterBuffer&& jitter_buffer);
            ~JitterBuffer();

            int PutMedia(const headers::MEDIA_DATA& media);
            const Sample* GetSample() const;
            void Clear();

        };
    }
}

#endif
