#include "ipsniffer.h"

#include "headers/ip.h"

#include <cstring>  // memcpy

#include <netdb.h>
#include <errno.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("IPSniffer", msg)

namespace mmx
{
    namespace sniffers
    {
        IPSniffer::IPSniffer(IPPacketPool& packet_pool) :
            packet_pool_(packet_pool),
            packet_(nullptr),
            saved_bytes_(0),
            state_(SS_START),
            wrapper_()
        {
            DLOGT(LOG_BEGIN("IPSniffer(&%x)"), DLOG_POINTER(&packet_pool));
        }

        IPSniffer::~IPSniffer()
        {
            DLOGT(LOG_BEGIN("~IPSniffer()"));
        }

        int IPSniffer::PutStream(const void* stream, int size, void* context)
        {
            int rc = -EINVAL;

            DLOGT(LOG_BEGIN("PutStream(%x, %d, %x)"), DLOG_POINTER(stream), size, DLOG_POINTER(context));

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
            else
            {
                DLOGE(LOG_BEGIN("PutStream(%x, %d, %x): invalid argument"), DLOG_POINTER(stream), size, DLOG_POINTER(context));
            }

            return rc;
        }


        int IPSniffer::Drop()
        {

            int rc = saved_bytes_;

            DLOGT(LOG_BEGIN("Drop(): state_ = %d, sb = %d, packet_ = %x"), state_, saved_bytes_, DLOG_POINTER(packet_));

            state_ = SS_START;

            if (packet_ != nullptr)
            {
                if (packet_->IsComplete())
                {
                    packet_pool_.Release(packet_);
                }
                packet_ = nullptr;
            }

            wrapper_.Reset();

            saved_bytes_ = 0;

            return rc;
        }

        int IPSniffer::Reset()
        {
            DLOGT(LOG_BEGIN("Reset(): packet_pool_(%d)"), packet_pool_.Count());

            int rc = packet_pool_.Count();

            Drop();

            packet_pool_.Reset();

            return rc;
        }

        bool IPSniffer::IsComplete() const
        {
            return state_ == SS_NEXT
                    && ((packet_ == nullptr)
                    || (packet_->IsComplete()));
        }

        bool IPSniffer::IsBad() const
        {
            return state_ == SS_BAD;
        }

        int IPSniffer::forceSniffer(const void* stream, int size)
        {
            // быстрый разбор пакета

            // отсутсвует копирование и автоматы... ожидаем > 99% нефрагментированного трафика

            // попробуем искать начало заголовка, но не более MAX_HEADER_LEN

            int rc = -EBADMSG;

            int drop = std::min(size, headers::MAX_HEADER_LEN);


            drop = IPPacket::FindStart(stream, drop);

            DLOGT(LOG_BEGIN("forceSniffer(%x, %d): drop = %d"), DLOG_POINTER(stream), size, drop);

            if (size != drop)
            {
                size -= drop;
                stream = (char*)(stream) + drop;

                // сразу проверяем пакет

                rc = IPPacket::CheckPacket(stream, size);

                // в rc должен вернуться размер пакета в байтах или -EBADMSG

                if (rc > 0)
                {
                    rc += drop;

                    headers::IP4HEADER &ip_header = *(headers::PIP4HEADER)stream;

                    // смещение в октетах (раскидано по разным байтам)

                    unsigned short offset = ip_header.off_lo;

                    offset = ip_header.off_hi | (offset << 8);

                    // в счет идут только нефрагментированые пакеты

                    if (ip_header.f_M == 0 && offset == 0)
                    {

                        unsigned short hlen = (ip_header.hlen << 2);

                        //force_stream_ = (char*)stream + hlen;

                        wrapper_.Reset(stream);

                        saved_bytes_ = rc - hlen;

                        DLOGT(LOG_BEGIN("forceSniffer(): packet is good, sb = %d!"), saved_bytes_);

                        state_ = SS_NEXT;


                    }

                    else
                    {
                        rc = -EBADMSG;
                    }


                }
            }

            return rc;
        }

        // фрагментный сниффер. Возвращает по факту обработаное количества байт

        int IPSniffer::fragmentSniffer(const void* stream, int size)
        {
            int rc = 0;

            char *p_src = (char*)stream;

            int len = size;

            bool f_stop = false;

            DLOGT(LOG_BEGIN("fragmentSniffer(%x, %d): state_ = %d, sb = %d"), DLOG_POINTER(stream), size, state_, saved_bytes_);

            while (!f_stop)
            {
                int process_bytes = 0;

                switch (state_)
                {
                    case SS_START:
                        {

                            // просто ищем первый байт заголовка


                            if (*p_src >= headers::MIN_IP_HEADER_START
                                           && *p_src <= headers::MAX_IP_HEADER_START)
                            {
                                saved_bytes_ = 1;

                                packet_ = nullptr;

                                *raw_header_ = *p_src;

                                state_ = SS_HEADER;
                            }


                            process_bytes = 1;
                        }
                        break;
                    case SS_HEADER:
                        {
                            // сюда попадаем после получения как минимум длины заголовка

                            // длину заголовка получаем в байтах и вносим поправку на сохраненный байт

                            unsigned short hlen = (header_.hlen << 2);

                            process_bytes = std::min(hlen - saved_bytes_, len);

                            std::memcpy(raw_header_ + saved_bytes_, p_src, process_bytes);

                            saved_bytes_ += process_bytes;

                            // если пакет дочитали

                            if (saved_bytes_ == hlen)
                            {
                                // априори обрекаем пакет на провал

                                state_ = SS_BAD;

                                if (IPPacket::CheckHeader(raw_header_, saved_bytes_) == saved_bytes_)
                                {
                                    // заголовок получен корректный, надо запросить пакет для сохранения pyload

                                    packet_ = packet_pool_.GetPacket(::ntohl(header_.src), ntohs(header_.id));

                                    if (packet_ != nullptr)
                                    {
                                        state_ = SS_PYLOAD;
                                    }
                                }

                            }

                        }
                        break;
                    case SS_PYLOAD:

                        state_ = SS_BAD;

                        if (packet_ != nullptr)
                        {
                            int ret = packet_->PutStream(p_src, len, &header_);

                            if (ret > 0)
                            {
                                saved_bytes_ += (process_bytes = ret);

                                if (saved_bytes_ == ntohs(header_.length))
                                {
                                    if (packet_->IsComplete())
                                    {
                                        state_ = SS_NEXT;
                                    }
                                    else
                                    {
                                        // Drop();
                                    }

                                }
                                else
                                {
                                    state_ = SS_PYLOAD;
                                }
                            }
                        }

                        break;

                    case SS_NEXT:
                    case SS_BAD:

                        f_stop = true;

                        break;


                }   // switch



                p_src += process_bytes;
                len -= process_bytes;

                f_stop |= len <= 0;

                DLOGT(LOG_BEGIN("fragmentSniffer(): state_ = %d, sb = %d, pb = %d, len = %d, stop = %d"), state_, saved_bytes_, process_bytes, len, f_stop);

            }   // while

            rc = size - len;

            return rc;
        }

        const IIPPacket* IPSniffer::GetPacket() const
        {
            const IIPPacket* rc = nullptr;

            if (IsComplete())
            {
                rc = packet_ == nullptr ?
                            (const IIPPacket*)&wrapper_ :
                            (const IIPPacket*)packet_;
            }

            return rc;
        }

    }
}
