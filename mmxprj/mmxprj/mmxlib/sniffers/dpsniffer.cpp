#include "dpsniffer.h"

#include <algorithm>
#include <cstring>

#include <errno.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("DataPackSniffer", msg)

namespace mmx
{
    namespace sniffers
    {
        DataPackSniffer::DataPackSniffer() :
            state_(SS_START1),
            saved_bytes_(0),
            work_data_pack_(nullptr)
        {
            DLOGT(LOG_BEGIN("DataPackSniffer()"));
        }

        DataPackSniffer::DataPackSniffer(DataPackSniffer&& sniffer) :
            state_(sniffer.state_),
            saved_bytes_(sniffer.saved_bytes_),
            data_pack_(sniffer.data_pack_),
            work_data_pack_(sniffer.work_data_pack_ == &sniffer.data_pack_
                            ? &data_pack_
                            : sniffer.work_data_pack_)
        {
            DLOGT(LOG_BEGIN("DataPackSniffer(&&%x)"), DLOG_POINTER(&sniffer));

            saved_bytes_ = 0;
            state_ = SS_START1;
            sniffer.work_data_pack_ = nullptr;
        }

        DataPackSniffer::~DataPackSniffer()
        {
            DLOGT(LOG_BEGIN("~DataPackSniffer()"));
        }

        int DataPackSniffer::PutStream(const void* stream, int size, void* hcontext)
        {
            int rc = -EINVAL;

            DLOGT(LOG_BEGIN("PutStream(%x, %d, %x): state_ = %d"), DLOG_POINTER(stream), size, DLOG_POINTER(hcontext), state_);

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
            else
            {
                DLOGE(LOG_BEGIN("PutStream(%x, %d, %x): invalid arguments"), DLOG_POINTER(stream), size, DLOG_POINTER(hcontext));
            }

            return rc;
        }

        int DataPackSniffer::Drop()
        {

            DLOGT(LOG_BEGIN("Drop(): state_ = %d, wdp = %x, sb = %d"), state_, DLOG_POINTER(work_data_pack_), saved_bytes_);

            int rc = saved_bytes_;

            state_ = SS_START1;

            work_data_pack_ = nullptr;

            saved_bytes_ = 0;

            return rc;
        }

        int DataPackSniffer::Reset()
        {

            DLOGT(LOG_BEGIN("Reset(): state_ = %d, wdp = %x, sb = %d"), state_, DLOG_POINTER(work_data_pack_), saved_bytes_);
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

        int DataPackSniffer::forceSniffer(const void* stream, int size)
        {
            int rc = -EBADMSG;

            DLOGT(LOG_BEGIN("forceSniffer(%x, %d)"), DLOG_POINTER(stream), size);

            if (size >= sizeof(headers::DATA_PACK_HEADER))
            {

                headers::DATA_PACK &dp = *(headers::PDATA_PACK)stream;

                // проверяем по формальным признакам пакет

                if (dp.header.magic == headers::DATA_PACK_MAGIC
                        && dp.header.length <= size
                        && DATA_PACK_TAIL(dp.header) == headers::DATA_PACK_MAGIC2)
                {

                    rc = dp.header.length;
                    work_data_pack_ = (headers::PDATA_PACK)stream;

                    state_ = SS_NEXT;

                    DLOGT(LOG_BEGIN("forceSniffer(): packet(%d) is good"), rc);

                }
                else
                {
                    DLOGD(LOG_BEGIN("forceSniffer(%x, %d): error format of stream"), DLOG_POINTER(stream), size);
                }
            }
            else
            {
                DLOGD(LOG_BEGIN("forceSniffer(%x, %d): error stream size"), DLOG_POINTER(stream), size);
            }

            return rc;
        }

        int DataPackSniffer::fragmentSniffer(const void* stream, int size)
        {
            int rc = 0;

            unsigned char *p_src = (unsigned char*)stream;

            int len = size;

            bool f_stop = false;

            static const unsigned short LO_MAGIC = headers::DATA_PACK_MAGIC & 0xFF;
            static const unsigned short HI_MAGIC = headers::DATA_PACK_MAGIC >> 8;

            static const unsigned short LO_MAGIC2 = headers::DATA_PACK_MAGIC2 & 0xFF;
            static const unsigned short HI_MAGIC2 = headers::DATA_PACK_MAGIC2 >> 8;

            DLOGT(LOG_BEGIN("fragmentSniffer(%x, %d)"), DLOG_POINTER(stream), size);

            while (!f_stop)
            {

                int process_bytes = 0;

                //unsigned short s = *p_src;

                DLOGT(LOG_BEGIN("fragmentSniffer(): state_ = %d, len = %d, pb = %d, sb = %d"), state_, len, process_bytes, saved_bytes_);

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
                                    DLOGE(LOG_BEGIN("fragmentSniffer(%x, %d): error data pack length (%d)"), DLOG_POINTER(stream), size, data_pack_.header.length);
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
                            else
                            {
                                DLOGE(LOG_BEGIN("fragmentSniffer(%x, %d): error stop magic word (%x)"), DLOG_POINTER(stream), size, *(unsigned short*)p_src);
                            }
                        }
                        else
                        {
                            if(*p_src == LO_MAGIC2)
                            {

                                state_ = SS_STOP2;

                                process_bytes = 1;

                            }
                            else
                            {
                                DLOGE(LOG_BEGIN("fragmentSniffer(%x, %d): error stop magic lo byte (%x)"), DLOG_POINTER(stream), size, *p_src);
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
                        else
                        {
                            DLOGE(LOG_BEGIN("fragmentSniffer(%x, %d): error stop magic hi byte (%x)"), DLOG_POINTER(stream), size, *p_src);
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

                        if (state_ == SS_NEXT)
                        {
                            DLOGT(LOG_BEGIN("fragmentSniffer(): fragment succes assemble"));
                        }
                    }

                    p_src += process_bytes;
                    len -= process_bytes;


                }
                else if (process_bytes < 0 || process_bytes > len)
                {
                    state_ = SS_BAD;
                    DLOGC(LOG_BEGIN("fragmentSniffer(%x, %d): desync stream!!!"), DLOG_POINTER(stream), size, *p_src);
                }


                f_stop |= len <= 0;
            }

            rc = size - len;

            DLOGT(LOG_BEGIN("fragmentSniffer(): process %d bytes of %d"), len, size);

            return rc;
        }
    }
}
