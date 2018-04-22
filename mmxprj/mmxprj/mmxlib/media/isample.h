#ifndef _MMX_MEDIA_I_SAMPLE_H
#define _MMX_MEDIA_I_SAMPLE_H

#include "headers/media.h"

#include "sniffers/irtppacket.h"

namespace mmx
{
    namespace media
    {
        class ISample
        {
        public:
            virtual ~ISample() {};
            virtual int PutSample(const sniffers::IRTPPacket& rtp, unsigned short pack_id, int timestamp = -1) = 0;
            virtual const headers::MEDIA_SAMPLE* GetSample() const= 0;
            virtual void Drop(int timestamp = -1) = 0;
        };
    }
}

#endif
