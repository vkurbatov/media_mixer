#include "sample.h"

#include <cstring>

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

            if (this != &sample)
            {
                data_ = std::move(sample.data_);
                is_valid_ = sample.is_valid_;
                sample.is_valid_ = false;
            }

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

        int Sample::GetCurrentTimestamp()
        {
            :: timeval tv;
            ::gettimeofday(&tv, 0);

            return (tv.tv_sec % 86400) * 1000 + (tv.tv_usec / 1000);
        }

        int Sample::GetDeltaTime(int timestamp)
        {
            int rc = GetCurrentTimestamp() - timestamp;

            return rc + (86400000) * (int)(rc < 0);
        }

        const headers::MEDIA_PROFILE_INFO& Sample::GetMediaInfo(headers::media_profile_t media_profile)
        {
            return headers::media_profiles_info_table[media_profile > headers::MEDIA_PROFILE_COUNT
                    ? headers::MEDIA_PROFILE_COUNT : media_profile];
        }

        // ISample
        int Sample::PutSample(const sniffers::IRTPPacket& rtp, unsigned short pack_id, int timestamp)
        {
            int rc = -EBADMSG;

            if (rtp.Header() != nullptr && rtp.Pyload() != nullptr)
            {
                const headers::RTP_HEADER &rtp_header = *(const headers::RTP_HEADER*)rtp.Header();

                rc = sizeof(headers::MEDIA_SAMPLE_HEADER) + rtp.PyloadSize();

                if (data_.size() < rc)
                {
                    data_.resize(rc);
                }

                headers::MEDIA_SAMPLE& sample = *(headers::MEDIA_SAMPLE*)data_.data();

                sample.header.length = rc;
                sample.header.magic = headers::MEDIA_SAMPLE_MAGIC;
                sample.header.packet_id = pack_id;
                sample.header.rtp_header = rtp_header;

                if (timestamp < 0)
                {
                    timestamp = GetCurrentTimestamp();
                }

                sample.header.timestamp = timestamp;

                std::memcpy(sample.media, rtp.Pyload(), rtp.PyloadSize());

            }
            is_valid_ = rc > 0;

            return rc;
        }

        const headers::MEDIA_SAMPLE* Sample::GetSample() const
        {
            return is_valid_
                    ? (const headers::MEDIA_SAMPLE*)data_.data()
                    : nullptr;
        }

        void Sample::Drop(int timestamp)
        {
            is_valid_ &= timestamp >= 0 && data_.size() > 0;

            if (is_valid_)
            {
                headers::MEDIA_SAMPLE& sample = *(headers::MEDIA_SAMPLE*)data_.data();

                is_valid_ = sample.header.timestamp > timestamp;
            }
        }

    }
}
