
#include "sangomasniffer.h"

#include <algorithm>
#include <cstring>

#include <errno.h>

namespace mmx
{
    namespace sniffers
    {

        SangomaSniffer::SangomaSniffer() :
            saved_bytes_(0),
            sangoma_(nullptr),
            state_(SS_START)
        {

        }

        // IStream
        int SangomaSniffer::PutStream(void* stream, int size, void* hcontext)
        {
            int rc = -EINVAL;

            if (stream != nullptr && size > 0)
            {
                rc = -EAGAIN;

                if (state_ == SS_START)
                {
                    rc = forceSniffer(stream, size);
                }

                if (state_ != SS_NEXT && state_ != SS_BAD)
                {
                    rc = fragmentSniffer(stream, size);
                }

            }

            return rc;
        }

        int SangomaSniffer::Drop()
        {
            sangoma_ = nullptr;
            saved_bytes_ = 0;
            state_ = SS_START;
        }

        int SangomaSniffer::Reset()
        {
            Drop();
        }

        bool SangomaSniffer::IsComplete() const
        {
            return state_ == SS_NEXT && sangoma_ != nullptr;
        }

        bool SangomaSniffer::IsBad() const
        {
            return state_ == SS_BAD;
        }

        const headers::SANGOMA_PACKET* SangomaSniffer::GetPacket()
        {
            return sangoma_;
        }

        int SangomaSniffer::forceSniffer(void* stream, int size)
        {
            int rc = -EBADMSG;

            headers::PSANGOMA_PACKET p = (headers::PSANGOMA_PACKET)stream;

            if (size >= sizeof(headers::SANGOMA_HEADER)
                    && p->header.length > sizeof(headers::SANGOMA_HEADER)
                    && p->header.length <= size )
            {

                rc = (sangoma_ = p)->header.length;

            }

            return rc;
        }

        int SangomaSniffer::fragmentSniffer(void* stream, int size)
        {

            int rc = 0;

            unsigned char *p_src = (unsigned char*)stream;

            int len = size;

            bool f_stop = false;


            while (!f_stop)
            {

                int process_bytes = 0;

                //unsigned short s = *p_src;

                switch(state_)
                {
                    case SS_START:

                        saved_bytes_ = 0;
                        state_ = SS_HEADER;

                        break;
                    case SS_HEADER:

                        process_bytes = std::min((int)sizeof(headers::SANGOMA_HEADER) - saved_bytes_, len);

                        if ((process_bytes + saved_bytes_) == sizeof(headers::SANGOMA_HEADER))
                        {

                            state_ = SS_PYLOAD;

                        }

                        break;

                    case SS_PYLOAD:
                        {

                            if (saved_bytes_ == sizeof(headers::SANGOMA_HEADER)
                                    && (sangoma_pack_.header.length < sizeof(headers::SANGOMA_HEADER)
                                    || sangoma_pack_.header.length > headers::MAX_PYLOAD_SIZE))
                            {

                                    state_ = SS_BAD;

                            }
                            else
                            {

                                process_bytes = std::min(sangoma_pack_.header.length - saved_bytes_, len);

                                if ((process_bytes + saved_bytes_) == sangoma_pack_.header.length)
                                {

                                    sangoma_ = &sangoma_pack_;
                                    state_ = SS_NEXT;

                                }

                            }
                        }
                        break;

                    case SS_NEXT:
                    case SS_BAD:

                        f_stop = true;

                        break;

                }

                if (process_bytes > 0 && process_bytes <= len)
                {

                    if (state_ != SS_START && state_ != SS_BAD)
                    {
                        if (process_bytes == 1)
                        {
                            *(raw_data + saved_bytes_) = *p_src;
                        }
                        else
                        {
                            std::memcpy(raw_data + saved_bytes_, p_src, process_bytes);
                        }

                        saved_bytes_ += process_bytes;
                    }


                    p_src += process_bytes;
                    len -= process_bytes;

                }
                else if (process_bytes < 0 || process_bytes > len)
                {
                    state_ = SS_BAD;
                }


                f_stop |= len <= 0;
            }

            rc = size - len;

            return rc;
        }

    }
}
