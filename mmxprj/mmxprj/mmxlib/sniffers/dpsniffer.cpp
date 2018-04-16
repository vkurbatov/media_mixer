#include "dpsniffer.h"

#include <algorithm>
#include <cstring>

#include <errno.h>

namespace mmx
{
    namespace sniffers
    {
        DataPackSniffer::DataPackSniffer() :
            state_(SS_START1),
            saved_bytes_(0),
            work_data_pack_(nullptr)
        {

        }

        DataPackSniffer::DataPackSniffer(DataPackSniffer&& sniffer) :
            state_(sniffer.state_),
            saved_bytes_(sniffer.saved_bytes_),
            data_pack_(sniffer.data_pack_),
            work_data_pack_(sniffer.work_data_pack_ == &sniffer.data_pack_
                            ? &data_pack_
                            : sniffer.work_data_pack_)
        {
            saved_bytes_ = 0;
            state_ = SS_START1;
            sniffer.work_data_pack_ = nullptr;
        }

        int DataPackSniffer::PutStream(void* stream, int size, void* hcontext)
        {
            int rc = -EINVAL;

            if (stream != nullptr && size > 0)
            {
                rc = -EAGAIN;

                if (state_ == SS_START1)
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

        int DataPackSniffer::Drop()
        {
            int rc = saved_bytes_;

            state_ = SS_START1;

            work_data_pack_ = nullptr;

            saved_bytes_ = 0;

            return rc;
        }

        int DataPackSniffer::Reset()
        {

            return Drop();

        }

        bool DataPackSniffer::IsComplete() const
        {
            return state_ == SS_NEXT &&  work_data_pack_ != nullptr;
        }

        bool DataPackSniffer::IsBad() const
        {
            return state_ == SS_BAD;
        }

        const headers::DATA_PACK* DataPackSniffer::GetDataPacket() const
        {
            return work_data_pack_;
        }

        int DataPackSniffer::forceSniffer(void* stream, int size)
        {
            int rc = -EBADMSG;

            if (size >= sizeof(headers::DATA_PACK_HEADER))
            {

                headers::DATA_PACK &dp = *(headers::PDATA_PACK)stream;

                // проверяем по формальным признакам пакет

                if (dp.header.magic == headers::DATA_PACK_MAGIC && dp.header.length <= size)
                {

                    //

                    rc = dp.header.length;
                    work_data_pack_ = (headers::PDATA_PACK)stream;

                    state_ = SS_NEXT;

                }
            }

            return rc;
        }

        int DataPackSniffer::fragmentSniffer(void* stream, int size)
        {
            int rc = 0;

            unsigned char *p_src = (unsigned char*)stream;

            int len = size;

            bool f_stop = false;

            static const unsigned short LO_MAGIC = headers::DATA_PACK_MAGIC & 0xFF;
            static const unsigned short HI_MAGIC = headers::DATA_PACK_MAGIC >> 8;

            static const unsigned short LO_MAGIC2 = headers::DATA_PACK_MAGIC2 & 0xFF;
            static const unsigned short HI_MAGIC2 = headers::DATA_PACK_MAGIC2 >> 8;

            while (!f_stop)
            {

                int process_bytes = 0;

                //unsigned short s = *p_src;

                switch(state_)
                {
                    case SS_START1:

                        saved_bytes_ = 0;

                        process_bytes = 1;

                        if (len > 1)
                        {

                            if (*(unsigned short*)p_src == headers::DATA_PACK_MAGIC)
                            {
                                process_bytes = 2;
                                state_ = SS_HEADER;

                            }
                        }
                        else
                        {
                            if(*p_src == LO_MAGIC)
                            {

                                state_ = SS_START2;

                            }
                        }


                        break;
                    case SS_START2:

                        if(*p_src == HI_MAGIC)
                        {

                            state_ = SS_HEADER;

                        }
                        else
                            state_ == SS_START1;

                        process_bytes = 1;

                        break;
                    case SS_HEADER:

                        process_bytes = std::min((int)sizeof(headers::DATA_PACK_HEADER) - saved_bytes_, len);

                        if ((process_bytes + saved_bytes_) == sizeof(headers::DATA_PACK_HEADER))
                        {
                            state_ = SS_PYLOAD;
                        }

                        break;

                    case SS_PYLOAD:
                        {
                            process_bytes = std::min(data_pack_.header.length - saved_bytes_ - (int)sizeof(headers::DATA_PACK_MAGIC2), len);

                            if ((process_bytes + saved_bytes_) == data_pack_.header.length - (int)sizeof(headers::DATA_PACK_MAGIC2))
                            {

                                if (data_pack_.header.length >= headers::MIN_PACKET_SIZE)
                                {
                                    state_ = SS_STOP1;
                                }
                                else
                                {
                                    state_ = SS_BAD;
                                }

                            }
                        }
                        break;
                    case SS_STOP1:

                        state_ = SS_BAD;

                        if (len > 1)
                        {
                            if (*(unsigned short*)p_src == headers::DATA_PACK_MAGIC2)
                            {
                                state_ = SS_NEXT;

                                work_data_pack_ = &data_pack_;

                                process_bytes = 2;
                            }
                        }
                        else
                        {
                            if(*p_src == LO_MAGIC2)
                            {

                                state_ = SS_STOP2;

                                process_bytes = 1;

                            }
                        }

                        break;
                    case SS_STOP2:

                        state_ = SS_BAD;

                        if(*p_src == HI_MAGIC2)
                        {

                            state_ = SS_NEXT;

                            work_data_pack_ = &data_pack_;

                            process_bytes = 1;

                        }


                        break;
                    case SS_NEXT:
                    case SS_BAD:

                        f_stop = true;

                        break;

                }

                if (process_bytes > 0 && process_bytes <= len)
                {

                    if (state_ != SS_START1 && state_ != SS_BAD)
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
