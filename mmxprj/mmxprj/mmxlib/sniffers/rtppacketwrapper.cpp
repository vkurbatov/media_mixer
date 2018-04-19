#include "rtppacketwrapper.h"

#include <netdb.h>  //htons,htonl...

namespace mmx
{
    namespace sniffers
    {

        RTPPacketWrapper::RTPPacketWrapper(const void* data, int size) :
            size_(0),
            header_(nullptr),
            ext_header_(nullptr),
            ext_data_(nullptr),
            pyload_(nullptr),
            pyload_size_(0)

        {
            Load(data, size);
        }

        bool RTPPacketWrapper::Load(const void* data, int size)
        {
            bool rc = false;

            if (data != nullptr && size >= sizeof(headers::RTP_HEADER))
            {
                header_ = (headers::PRTP_HEADER)(data);

                if (header_->ver == headers::RTP_VERSION)
                {
                    // размер заголовка

                    int len = (int)sizeof(headers::PRTP_HEADER) + header_->csrc_count * 4;

                    if (len <= size)
                    {

                        if (header_->extension)
                        {

                            ext_header_ = (headers::PRTP_EXTENSION_HEADER)((const char *)data + len);
                            len += sizeof(headers::RTP_EXTENSION_HEADER);
                            ext_data_ = (const char*)(ext_header_) + len;
                            len += ::ntohs(ext_header_->ehl) * 4;

                        }

                        if (len <= size)
                        {
                            pyload_ = (char*)data + len;

                            pyload_size_ = size - len;

                            if (header_->padding != 0)
                            {
                                unsigned short L = ::ntohs(*(unsigned short*)(((const char*)data) + size - 2));

                                pyload_size_ -= L + 2;
                            }


                            if (rc = (pyload_size_ >= 0))
                            {

                                size_ = size;

                            }

                        }

                    }

                }
            }

            if (rc == false)
            {
                header_ = nullptr;
                ext_header_ = nullptr;
                ext_data_ = nullptr;
                size_ = pyload_size_ = 0;
                pyload_ = nullptr;
            }

            return rc;
        }

        const headers::RTP_HEADER* RTPPacketWrapper::Header() const
        {
            return header_;
        }

        const headers::RTP_EXTENSION_HEADER* RTPPacketWrapper::ExtensionHeader() const
        {
            return ext_header_;
        }

        const void* RTPPacketWrapper::ExtensionData() const
        {
            return ext_data_;
        }

        const void* RTPPacketWrapper::Pyload() const
        {
            return pyload_;
        }

        int RTPPacketWrapper::PyloadSize() const
        {
            return size_ - (int)((const char*)pyload_ - (const char*)header_);
        }

        int RTPPacketWrapper::Size() const
        {
            return size_;
        }
    }
}
