#include "sample.h"

#include <cstring>

#include <errno.h>
#include <sys/time.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("Sample", msg)

namespace mmx
{
    namespace media
    {
        Sample::Sample() :
            data_(0),
            is_valid_(false)
        {
            DLOGT(LOG_BEGIN("Sample()"));
        }

        Sample::Sample(Sample&& sample) :
            data_(std::move(sample.data_)),
            is_valid_(sample.is_valid_)
        {
            sample.is_valid_ = false;
            DLOGT(LOG_BEGIN("Sample(&&%x)"), DLOG_POINTER(&sample));
        }

        Sample& Sample::operator= (Sample&& sample)
        {

            if (this != &sample)
            {
                DLOGT(LOG_BEGIN("operator=(&&%x)"), DLOG_POINTER(&sample));

                data_ = std::move(sample.data_);
                is_valid_ = sample.is_valid_;
                sample.is_valid_ = false;
            }

            return *this;
        }

        Sample::~Sample()
        {
            DLOGT(LOG_BEGIN("~Sample(): with data size = %d"), data_.size());
        }

        Sample& Sample::Swap(Sample& sample)
        {
            DLOGT(LOG_BEGIN("Swap(&&%x)"), DLOG_POINTER(&sample));
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

            DLOGT(LOG_BEGIN("PutSample(&%x, %d, %d)"), DLOG_POINTER(&rtp), pack_id, timestamp);

            if (rtp.Header() != nullptr && rtp.Pyload() != nullptr)
            {
                const headers::RTP_HEADER &rtp_header = *(const headers::RTP_HEADER*)rtp.Header();

                if (rtp_header.pyload_type == headers::MA_PCMA)
                {

                    int pl_size = rtp.PyloadSize();
                    const void* pl_data = rtp.Pyload();
                    rc = sizeof(headers::MEDIA_SAMPLE_HEADER) + pl_size;

                    if (data_.size() < rc)
                    {
                        DLOGT(LOG_BEGIN("PutSample() data_ resize %d -> %d"), data_.size(), rc);
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

                    if (pl_data != nullptr && pl_size > 0)
                    {
                        DLOGT(LOG_BEGIN("PutSample(): pyload save %d bytes!"), pl_size);
                        std::memcpy(sample.media, pl_data, pl_size);
                    }
                    else
                    {
                        DLOGI(LOG_BEGIN("PutSample(&%x, %d, %d): pyload field is empty: %x,%d"), DLOG_POINTER(&rtp), pack_id, timestamp, pl_data, pl_size);
                    }

                }
                else
                {
                    DLOGI(LOG_BEGIN("PutSample(&%x, %d, %d): Media is not PCMA format"), DLOG_POINTER(&rtp), pack_id, timestamp);
                }

            }
            else
            {
                DLOGI(LOG_BEGIN("PutSample(&%x, %d, %d): Media data is not rtp format"), DLOG_POINTER(&rtp), pack_id, timestamp);

                is_valid_ = false;
            }
            is_valid_ = rc > 0;

            return rc;
        }

        const headers::MEDIA_SAMPLE* Sample::GetMediaSample() const
        {
            return is_valid_
                    ? (const headers::MEDIA_SAMPLE*)data_.data()
                    : nullptr;
        }

        void Sample::Drop(int timestamp)
        {
            DLOGT(LOG_BEGIN("Drop(%d): ts = %d, valid = %d"), timestamp, is_valid_);

            is_valid_ &= timestamp >= 0 && data_.size() > 0;

            if (is_valid_)
            {

                headers::MEDIA_SAMPLE& sample = *(headers::MEDIA_SAMPLE*)data_.data();

                is_valid_ = sample.header.timestamp > timestamp;
            }

        }

    }
}
