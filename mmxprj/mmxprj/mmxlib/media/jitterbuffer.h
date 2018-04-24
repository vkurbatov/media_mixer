#ifndef _MMX_MEDIA_JITTER_H
#define _MMX_MEDIA_JITTER_H

#include "sample.h"
#include "sniffers/irtppacket.h"

#include <vector>

namespace mmx
{
    namespace media
    {
        class JitterBuffer
        {
            unsigned short              frame_size_;
            unsigned short              freq_;
            std::vector<Sample>         samples_;
            unsigned int                ssrc_;
            unsigned int                ts_rtp_;
            int                         ts_bias_;



        public:
            JitterBuffer(unsigned short frame_size, unsigned short freq, int size = 5);
            JitterBuffer(JitterBuffer&& jitter_buffer);
            JitterBuffer& operator=(JitterBuffer&& jitter_buffer);
            ~JitterBuffer();

            int PutMedia(sniffers::IRTPPacket& rtp, unsigned short pack_id, int timestamp = -1);
            const Sample* GetSample(int timestamp = -1) const;
            void Drop(int timestamp = -1);
            bool IsEmpty() const;

        private:

            const Sample* checkSample(const Sample& sample) const;


        };
    }
}

#endif
