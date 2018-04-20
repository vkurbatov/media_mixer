#ifndef _MMX_MEDIA_I_SAMPLE_H
#define _MMX_MEDIA_I_SAMPLE_H

#include "headers/media.h"

namespace mmx
{
    namespace media
    {
        class ISample
        {
        public:
            virtual ~ISample() {};
            virtual int PutSample(const void* rtp_packet, int size, unsigned short pack_id, int timestamp = -1) = 0;
            virtual const headers::MEDIA_SAMPLE* GetSample() = 0;
            virtual void Drop() = 0;
        };
    }
}

#endif
