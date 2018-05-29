#ifndef _MMX_MEDIA_SAMPLE_H
#define _MMX_MEDIA_SAMPLE_H

#include "isample.h"

#include <vector>

namespace mmx
{
    namespace media
    {

        class Sample : public ISample
        {

            std::vector<char>       data_;
            bool                    is_valid_;

        public:
            Sample();
            Sample(Sample&& sample);
            Sample& operator= (Sample&& sample);
            ~Sample();

            Sample& Swap(Sample& sample);
            static int GetCurrentTimestamp();
            static int GetDeltaTime(int timestamp);
            static const headers::MEDIA_PROFILE_INFO& GetMediaInfo(headers::media_profile_t media_profile);

            unsigned char _dummed_;

            // ISample
            int PutSample(const sniffers::IRTPPacket& rtp, unsigned short pack_id, int timestamp = -1) override;
            const headers::MEDIA_SAMPLE* GetMediaSample() const override;
            void Drop(int timestamp = -1) override;
        };
    }
}

#endif
