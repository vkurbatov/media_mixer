#include "sample.h"

#include <cstring>

#include "sniffers/rtppacketwrapper.h"

#include <errno.h>
#include <sys/time.h>

namespace mmx
{
    namespace media
    {
        Sample::Sample() :
            data_(0),
            is_valid_(false)
        {

        }

        Sample::Sample(Sample&& sample) :
            data_(std::move(sample.data_)),
            is_valid_(sample.is_valid_)
        {
            sample.is_valid_ = false;
        }

        Sample& Sample::operator= (Sample&& sample)
        {
            data_ = std::move(sample.data_);
            is_valid_ = sample.is_valid_;
            sample.is_valid_ = false;

            return *this;
        }

        Sample::~Sample()
        {

        }

        Sample& Sample::Swap(Sample& sample)
        {
            data_.swap(sample.data_);
            std::swap(is_valid_, sample.is_valid_);
            return *this;
        }

        // ISample
        int Sample::PutSample(const void* rtp_packet, int size, unsigned short pack_id, int timestamp)
        {
            int rc = -EBADMSG;

            sniffers::RTPPacketWrapper rtp(rtp_packet, size);

            if (rtp.Header() != nullptr && rtp.Pyload() != nullptr)
            {
                const headers::RTP_HEADER &rtp_header = *(const headers::RTP_HEADER*)rtp.Header();

                rc = sizeof(headers::MEDIA_SAMPLE_HEADER) + rtp.PyloadSize();

                if (data_.size() < rc)
                {
                    data_.resize(rc);
                }

                headers::MEDIA_SAMPLE& sample = *(headers::MEDIA_SAMPLE*)data_.data();

                sample.header.magic = headers::MEDIA_SAMPLE_MAGIC;
                sample.header.packet_id = pack_id;
                sample.header.rtp_header = rtp_header;

                if (timestamp < 0)
                {
                    :: timeval tv;
                    ::gettimeofday(&tv, 0);

                    timestamp = (tv.tv_sec % 86400) * 1000 + (tv.tv_usec / 1000);
                }

                sample.header.timestamp = timestamp;

                std::memcpy(sample.media, rtp.Pyload(), rtp.PyloadSize());

            }

            is_valid_ = rc > 0;

            return rc;
        }

        const headers::MEDIA_SAMPLE* Sample::GetSample()
        {
            return is_valid_
                    ? (const headers::MEDIA_SAMPLE*)data_.data()
                    : nullptr;
        }

        void Sample::Drop()
        {
            is_valid_ = false;
        }

    }
}
