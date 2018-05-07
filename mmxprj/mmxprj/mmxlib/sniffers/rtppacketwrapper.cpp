#include "rtppacketwrapper.h"

#include <netdb.h>  //htons,htonl...
#include <errno.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("RTPPacketWrapper", msg)

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
            DLOGT(LOG_BEGIN("RTPPacketWrapper(%x, %d)"), DLOG_POINTER(data), size);
            Load(data, size);
        }

        RTPPacketWrapper::~RTPPacketWrapper()
        {
            DLOGT(LOG_BEGIN("~RTPPacketWrapper()"));
        }

        bool RTPPacketWrapper::Load(const void* data, int size)
        {
            bool rc = false;

            DLOGT(LOG_BEGIN("Load(%x, %d)"), DLOG_POINTER(data), size);

            if (data != nullptr && size >= sizeof(headers::RTP_HEADER))
            {
                header_ = (headers::PRTP_HEADER)(data);

                if (header_->ver == headers::RTP_VERSION)
                {
                    // размер заголовка

                    int len = (int)sizeof(headers::RTP_HEADER) + header_->csrc_count * 4;

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

                                DLOGT(LOG_BEGIN("Load(): packet with padding, L = %d"), L);
                            }


                            if (rc = (pyload_size_ >= 0))
                            {

                                size_ = size;

                                DLOGT(LOG_BEGIN("Load(): rtp packet is good, size = %d, p_size = %"), size_, pyload_size_);

                            }

                        }
                        else
                        {
                            errno = -EBADMSG;
                            DLOGD(LOG_BEGIN("Load(%x, %d): packet with extension is too long, len = %d"), DLOG_POINTER(data), size, len);
                        }
                    }
                    else
                    {
                        errno = -EBADMSG;
                        DLOGD(LOG_BEGIN("Load(%x, %d): packet is too long, len = %d"), DLOG_POINTER(data), size, len);
                    }
                }
                else
                {
                    errno = -EBADMSG;
                    DLOGD(LOG_BEGIN("Load(%x, %d): deffered version = %d"), DLOG_POINTER(data), size, header_->ver);
                }
            }
            else
            {
                errno = -EINVAL;
                DLOGE(LOG_BEGIN("Load(%x, %d): invalid arguments"), DLOG_POINTER(data), size);
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
