#include "jitterbuffer.h"

#include <cstring>
#include <netdb.h>  // ::htonl

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
            jitter_buffer.Drop();
        }

        JitterBuffer& JitterBuffer::operator=(JitterBuffer&& jitter_buffer)
        {
            frame_size_= jitter_buffer.frame_size_;
            freq_ = jitter_buffer.freq_;
            samples_= std::move(jitter_buffer.samples_);
            ssrc_= jitter_buffer.ssrc_;
            ts_bias_= jitter_buffer.ts_bias_;
            ts_rtp_ = jitter_buffer.ts_rtp_;

            jitter_buffer.Drop();

            return *this;

        }

        JitterBuffer::~JitterBuffer()
        {

        }

        int JitterBuffer::PutMedia(sniffers::IRTPPacket& rtp, unsigned short pack_id, int timestamp)
        {
            int rc = 0;

            //const headers::MEDIA_SAMPLE* smpl = nullptr

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

            }


            return rc;
        }

        const Sample* JitterBuffer::GetSample(int timestamp) const
        {
            const Sample* rc = nullptr;

            if (timestamp < 0)
            {
                timestamp = (Sample::GetCurrentTimestamp() - (frame_size_ * (samples_.size() + 1))) % 86400000;
            }

            if (ts_bias_ >= 0)
            {
                int idx = ((timestamp / frame_size_) + ts_bias_) % samples_.size();

                rc = checkSample(samples_[idx]);

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

            if (smpl != nullptr
                    && smpl->header.rtp_header.ssrc == ssrc_)
            {
                unsigned int ts_delta = ts_rtp_ - ::htonl(smpl->header.rtp_header.timestamp);

                if (ts_delta < frame_size_ * (samples_.size() + 1))
                {
                    rc = &sample;
                }
            }

            return rc;
        }
    }
}
