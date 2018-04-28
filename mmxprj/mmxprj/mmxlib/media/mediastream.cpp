#include "mediastream.h"
#include "sniffers/rtppacketwrapper.h"

#include <cstring>

#include "errno.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("MediaStream", msg)


namespace mmx
{
    namespace media
    {
        MediaStream::MediaStream(unsigned int address, unsigned short port) :
            pack_id_(0),
            ref_count_(0),
            address_(address),
            port_(port),
            jitter_(20,8000,6)
        {
            DLOGT(LOG_BEGIN("MediaStream(%d, %d)"), address, port);
        }

        MediaStream::MediaStream(MediaStream&& mediastream) :
            pack_id_(mediastream.pack_id_),
            ref_count_(mediastream.ref_count_),
            address_(mediastream.address_),
            port_(mediastream.port_),
            jitter_(std::move(mediastream.jitter_))

        {

            DLOGT(LOG_BEGIN("MediaStream(&&%x)"), DLOG_POINTER(&mediastream));

            mediastream.pack_id_ = 0;
            mediastream.ref_count_ = 0;
            mediastream.address_ = 0;
            mediastream.port_ = 0;

        }

        MediaStream& MediaStream::operator=(MediaStream&& mediastream)
        {
            if (this != &mediastream)
            {
                pack_id_ = mediastream.pack_id_;
                ref_count_ = mediastream.ref_count_;
                address_ = mediastream.address_;
                port_ = mediastream.port_;
                jitter_ = std::move(mediastream.jitter_);

                DLOGT(LOG_BEGIN("operator=(&&%x)"), DLOG_POINTER(&mediastream));

                mediastream.pack_id_ = 0;
                mediastream.ref_count_ = 0;
                mediastream.address_ = 0;
                mediastream.port_ = 0;
            }

            return *this;
        }


        MediaStream::~MediaStream()
        {
            DLOGT(LOG_BEGIN("~MediaStream()"));
        }

        int MediaStream::PutSample(const mmx::headers::MEDIA_DATA& media)
        {
            int rc = 0;

            mmx::sniffers::RTPPacketWrapper rtp(&media.media, media.header.length - sizeof(media.header));

            if (rtp.Header() != nullptr)
            {

                rc = jitter_.PutMedia(rtp, ++pack_id_, media.header.timestamp);

            }
            else
            {
                DLOGW(LOG_BEGIN("PutSample(&%x): media data is not rtp"), DLOG_POINTER(&media));
            }

            return rc;
        }

        const headers::MEDIA_SAMPLE* MediaStream::GetMediaSample() const
        {

            auto smpl = jitter_.GetSample(Sample::GetCurrentTimestamp() - 60);

            return smpl != nullptr  ? smpl->GetMediaSample() : nullptr;

        }

        void MediaStream::Clear()
        {
            jitter_.Drop();
        }
    }
}
