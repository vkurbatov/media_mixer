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
            ~Sample() override;

            Sample& Swap(Sample& sample);

            // ISample
            int PutSample(const void* rtp_packet, int size, unsigned short pack_id, int timestamp = -1) override;
            const headers::MEDIA_SAMPLE* GetSample() override;
            void Drop() override;
        };
    }
}

#endif
