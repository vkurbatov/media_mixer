#include "jitterbuffer.h"

#include <cstring>
#include <netdb.h>  // ::htonl
#include <errno.h>


#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("JitterBuffer", msg)


namespace mmx
{
    namespace media
    {
        JitterBuffer::JitterBuffer(unsigned short frame_size, unsigned short freq, int size) :
            frame_size_(frame_size),
            freq_(freq),
            samples_(size),
            ssrc_(0),
            ts_bias_(-1),
            ts_rtp_(0)
        {

            DLOGT(LOG_BEGIN("JitterBuffer(%d, %d, %d)"), frame_size, freq, size);
            //std::memset(&rtp_header_, 0, sizeof(rtp_header_));
        }

        JitterBuffer::JitterBuffer(JitterBuffer&& jitter_buffer) :
            frame_size_(jitter_buffer.frame_size_),
            freq_(jitter_buffer.freq_),
            samples_(std::move(jitter_buffer.samples_)),
            ssrc_(jitter_buffer.ssrc_),
            ts_bias_(jitter_buffer.ts_bias_),
            ts_rtp_(jitter_buffer.ts_rtp_)

        {
            DLOGT(LOG_BEGIN("JitterBuffer(%x)"), DLOG_POINTER(&jitter_buffer));
            jitter_buffer.Drop();
        }

        JitterBuffer& JitterBuffer::operator=(JitterBuffer&& jitter_buffer)
        {

            DLOGT(LOG_BEGIN("operator=(%x)"), DLOG_POINTER(&jitter_buffer));

            if (this != & jitter_buffer)
            {
                frame_size_= jitter_buffer.frame_size_;
                freq_ = jitter_buffer.freq_;
                samples_= std::move(jitter_buffer.samples_);
                ssrc_= jitter_buffer.ssrc_;
                ts_bias_= jitter_buffer.ts_bias_;
                ts_rtp_ = jitter_buffer.ts_rtp_;

                jitter_buffer.Drop();
            }

            return *this;

        }

        JitterBuffer::~JitterBuffer()
        {
            DLOGT(LOG_BEGIN("~JitterBuffer()"));
        }

        int JitterBuffer::PutMedia(sniffers::IRTPPacket& rtp, unsigned short pack_id, int timestamp)
        {
            int rc = 0;

            //const headers::MEDIA_SAMPLE* smpl = nullptr

            DLOGT(LOG_BEGIN("PutMedia(%x, %d, %d)"), DLOG_POINTER(&rtp), pack_id, timestamp);

            if (rtp.Header()!= nullptr)
            {
                const headers::RTP_HEADER& rtp_header = *rtp.Header();

                int size = samples_.size();

                int dts = ((int)frame_size_ * (int)freq_) / 1000;

                ts_rtp_ = ::ntohl(rtp_header.timestamp);

                int idx = (ts_rtp_ / dts) % size;

                if (timestamp < 0)
                {
                    timestamp = Sample::GetCurrentTimestamp();
                }                              

                if (ts_bias_ < 0 || ssrc_ != rtp_header.ssrc)
                {
                    ts_bias_ = (idx - ((timestamp / frame_size_) % size) + size) % size;
                    ssrc_ = rtp_header.ssrc;
                }

                rc = samples_[idx].PutSample(rtp, pack_id, timestamp);

                if (rc >= 0)
                {
                    DLOGD(LOG_BEGIN("PutMedia(%x, %d, %d): samples_[%d] put media success, rc = %d"), DLOG_POINTER(&rtp), pack_id, timestamp, idx, rc);
                }
                else
                {
                    DLOGE(LOG_BEGIN("PutMedia(%x, %d, %d): samples_[%d] put media error, rc = %d"), DLOG_POINTER(&rtp), pack_id, timestamp, idx, rc);
                }

            }
            else
            {
                rc = -EBADMSG;
                DLOGE(LOG_BEGIN("PutMedia(%x, %d, %d) Bad rtp header, rc = %d"), DLOG_POINTER(&rtp), pack_id, timestamp, rc);
            }

            return rc;
        }

        const Sample* JitterBuffer::GetSample(int timestamp) const
        {
            const Sample* rc = nullptr;

            if (timestamp < 0)
            {
                timestamp = (Sample::GetCurrentTimestamp() - (frame_size_ * (samples_.size() + 2))) % 86400000;

                DLOGT(LOG_BEGIN("GetSample(): calculate timestamp = %d"), timestamp);
            }

            if (ts_bias_ >= 0)
            {
                int idx = ((timestamp / frame_size_) + ts_bias_) % samples_.size();

                rc = checkSample(samples_[idx]);

                DLOGT(LOG_BEGIN("GetSample(): samples_[%d], rc = %x)"), idx, DLOG_POINTER(rc));
            }
            else
            {
                DLOGD(LOG_BEGIN("GetSample(): jitter buffer is empty"));
            }

            return rc;
        }

        bool JitterBuffer::IsEmpty() const
        {
            return ts_bias_ < 0;
        }

        void JitterBuffer::Drop(int timestamp)
        {
            if (timestamp < 0)
            {
                ts_bias_ = -1;
            }

            for (auto& s: samples_)
            {
                s.Drop(timestamp);
            }

        }

        const Sample* JitterBuffer::checkSample(const Sample& sample) const
        {
            const Sample* rc = nullptr;

            auto smpl = sample.GetMediaSample();

            if (smpl != nullptr)
            {
                if (smpl->header.rtp_header.ssrc == ssrc_)
                {

                    rc = &sample;

                }
                else
                {
                    DLOGI(LOG_BEGIN("checkSample(%x): drop sample, different ssrc %d != %d"), DLOG_POINTER(&sample), smpl->header.rtp_header.ssrc, ssrc_);
                }
            }
            else
            {
                DLOGI(LOG_BEGIN("checkSample(%x): sample is empty"), DLOG_POINTER(&sample));
            }

            return rc;
        }
    }
}
