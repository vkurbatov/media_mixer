#include "mediastream.h"
#include "sniffers/rtppacketwrapper.h"

#include <cstring>

#include "errno.h"

namespace mmx
{
    namespace media
    {
        MediaStream::MediaStream(unsigned int address, unsigned short port) :
            data_(0),
            pack_id_(0),
            ref_count_(0),
            address_(address),
            port_(port)
        {

        }

        MediaStream::MediaStream(MediaStream&& mediastream) :
            data_(std::move(mediastream.data_)),
            pack_id_(mediastream.pack_id_),
            ref_count_(mediastream.ref_count_),
            address_(mediastream.address_),
            port_(mediastream.port_)
        {

            mediastream.pack_id_ = 0;
            mediastream.ref_count_ = 0;
            mediastream.address_ = 0;
            mediastream.port_ = 0;

        }

        MediaStream& MediaStream::operator=(MediaStream&& mediastream)
        {
            if (this != &mediastream)
            {
                data_ = std::move(mediastream.data_);
                pack_id_ = mediastream.pack_id_;
                ref_count_ = mediastream.ref_count_;
                address_ = mediastream.address_;
                port_ = mediastream.port_;

                mediastream.pack_id_ = 0;
                mediastream.ref_count_ = 0;
                mediastream.address_ = 0;
                mediastream.port_ = 0;
            }

            return *this;
        }

        int MediaStream::PutSample(const mmx::headers::MEDIA_DATA& media)
        {
            int rc = 0;

            mmx::sniffers::RTPPacketWrapper rtp(&media.media, media.header.length - sizeof(media.header));

            if (rtp.Header() != nullptr)
            {
                int total_size = rtp.PyloadSize() + sizeof(mmx::headers::MEDIA_SAMPLE_HEADER);

                const mmx::headers::RTP_HEADER& rtp_header = *rtp.Header();

                if (total_size > data_.size())
                {
                    data_.resize(total_size);
                }

                mmx::headers::MEDIA_SAMPLE& sample = *(mmx::headers::MEDIA_SAMPLE*)data_.data();

                sample.header.magic = mmx::headers::MEDIA_SAMPLE_MAGIC;
                sample.header.length = (unsigned short)total_size;
                sample.header.rtp_header = rtp_header;
                //sample.header.net_points = media.header.net_points;
                sample.header.packet_id = ++pack_id_;
                sample.header.timestamp = media.header.timestamp;


                std::memcpy(sample.media, rtp.Pyload(), rtp.PyloadSize());

                rc = total_size;

            }

            return rc;
        }

        const mmx::headers::MEDIA_SAMPLE* MediaStream::GetSample() const
        {

            const mmx::headers::MEDIA_SAMPLE* rc = nullptr;

            if (data_.size() >= sizeof(mmx::headers::MEDIA_SAMPLE))
            {
                rc = (const mmx::headers::MEDIA_SAMPLE*)data_.data();

                if (rc->header.length > data_.size())
                {

                    rc = nullptr;

                }

            }

            return rc;

        }

        void MediaStream::Clear()
        {
            data_.clear();
        }
    }
}
