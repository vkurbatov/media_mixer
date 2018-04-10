#include "rtppacketwrapper.h"

namespace mmx
{
    namespace sniffers
    {

        RTPPacketWrapper::RTPPacketWrapper(void* data, int size) :
            data_(data),
            size_(size)
        {

        }

        bool RTPPacketWrapper::Load(void* data, int size)
        {

        }

        const headers::RTP_HEADER* RTPPacketWrapper::Header() const
        {

        }

        const headers::RTP_EXTENSION_HEADER* RTPPacketWrapper::ExtensionHeader() const
        {

        }

        const void* RTPPacketWrapper::ExtensionData() const
        {

        }

        const void* RTPPacketWrapper::Pyload() const
        {

        }

        int RTPPacketWrapper::Size() const
        {

        }

        bool RTPPacketWrapper::check()
        {
            bool rc = false;

            if (data_ != nullptr && size_ >= sizeof(headers::RTP_HEADER))
            {
                headers::RTP_HEADER &rtp = *(headers::PRTP_HEADER)(data_);

                if (rtp.ver == headers::RTP_VERSION)
                {
                    int len = sizeof(headers::PRTP_HEADER) + rtp.csrc_count * 4;

                    if (len <= size)
                    {

                        if (rtp.extension)
                        {
                            headers::RTP_EXTENSION_HEADER &ex_rtp = *(headers::PRTP_EXTENSION_HEADER)((char *)data_ + len);
                        }

                    }

                }
            }

            if (rc == false)
            {
                data_ = nullptr;
                size_ = 0;
            }

            return rc;
        }

    }
}
