#ifndef _MMX_MEDIA_I_JITTER_H
#define _MMX_MEDIA_I_JITTER_H

#include "headers/media.h"

namespace mmx
{
    namespace media
    {
        class IJitter
        {
        public:
            virtual ~IJitter() {}

            virtual int PutSample(Sample&& sample) = 0;
            virtual const mmx::headers::MEDIA_SAMPLE* GetSample() const = 0;
            virtual void Clear() = 0;
        };
    }
}
