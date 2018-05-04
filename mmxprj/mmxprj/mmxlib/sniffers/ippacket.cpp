#include <algorithm>
#include <cstring>  // memcpy, memset

#include "ippacket.h"

#include <errno.h>
#include <netdb.h> // htonl, htons

#include "tools/timer.h"

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("IPPacket", msg)

namespace mmx
{
    namespace sniffers
    {
        IPPacket::IPPacket(unsigned int src_address, unsigned short packet_id) :
            needs_bytes_(-1),
            saved_bytes_(0),
            frame_bytes_(-1),
            data_(DEFAULT_MTU_SIZE),
            frag_set_(MAX_FRAGMEN_VALUE),
            src_addr_(src_address),
            pack_id_(packet_id),
            t_stamp_(tools::Timer::GetTicks()),
            offset_(0)
        {
            DLOGT(LOG_BEGIN("IPPacket(%d, %d)"), src_address, packet_id);
        }

        IPPacket::IPPacket(IPPacket&& packet) :
            needs_bytes_(packet.needs_bytes_),
            saved_bytes_(packet.saved_bytes_),
            frame_bytes_(packet.frame_bytes_),
            data_(std::move(packet.data_)),
            frag_set_(std::move(packet.frag_set_)),
            src_addr_(packet.src_addr_),
            pack_id_(packet.pack_id_),
            t_stamp_(packet.t_stamp_),
            offset_(packet.offset_)
        {

            DLOGT(LOG_BEGIN("IPPacket(&&%x)"), DLOG_POINTER(&packet));

            packet.needs_bytes_ = -1;
            packet.saved_bytes_ = 0;
            packet.frame_bytes_ = -1;
            packet.src_addr_ = 0;
            packet.pack_id_ = -1;
            packet.offset_ = 0;

        }

        IPPacket& IPPacket::operator=(IPPacket&& packet)
        {
            DLOGT(LOG_BEGIN("operator=(&&%x)"), DLOG_POINTER(&packet));

            needs_bytes_ = packet.needs_bytes_;
            saved_bytes_= packet.saved_bytes_;
            frame_bytes_= packet.frame_bytes_;
            data_=std::move(packet.data_);
            frag_set_=std::move(packet.frag_set_);
            src_addr_= packet.src_addr_;
            pack_id_= packet.pack_id_;
            t_stamp_ = packet.t_stamp_;
            offset_=  packet.offset_;

            packet.needs_bytes_ = -1;
            packet.saved_bytes_ = 0;
            packet.frame_bytes_ = -1;
            packet.src_addr_ = 0;
            packet.pack_id_ = -1;
            packet.offset_ = 0;

            return *this;
        }

        IPPacket::~IPPacket()
        {
            DLOGT(LOG_BEGIN("~IPPacket()"));
        }

        bool IPPacket::operator==(const IPPacket& packet)
        {
            return this == &packet;
        }

        int IPPacket::FindStart(const void* stream, int size)
        {
            int rc = -EINVAL;

            unsigned char* p = (unsigned char*) stream;

            if (stream != nullptr && size > 0)
            {
                rc = 0;

                while (rc < size
                       && (*p < headers::MIN_IP_HEADER_START
                       || *p > headers::MAX_IP_HEADER_START)
                       )
                {
                    p++;
                    rc++;
                }

            }
            else
            {
                DLOGE("IPPacket::FindStart(%x, %d): invalid arguments", DLOG_POINTER(stream), size);
            }

            return rc;
        }

        // проверяет IP-заголовок по указателю. В случае успеха, возвращает длину заголовка в байтах       

        int IPPacket::CheckHeader(const void* stream, int size)
        {
            int rc = -EINVAL;

            DLOGT("IPPacket::CheckHeader(%x, %d)", DLOG_POINTER(stream), size);

            if (stream != nullptr && size >= sizeof(headers::IP4HEADER))
            {
                rc = -EBADMSG;

                headers::IP4PACKET &pack = *(headers::PIP4PACKET)stream;

                const char &hd_start = *(const char*)stream;

                // 1-й уровень проверки (по первому байту)

                if (hd_start >= headers::MIN_IP_HEADER_START
                        && hd_start <= headers::MAX_IP_HEADER_START)
                {
                    // 2 - уровень проверки (по длине)

                    unsigned short len = (pack.header.length << 8) | (pack.header.length >> 8);

                    if (len >= sizeof(headers::IP4HEADER))
                    {

                        // 3 - й уровень проверки, по контрольной сумме заголовка

                        unsigned short hlen = pack.header.hlen << 2; //переводим в байты

                        if ((rc = CheckSumm(stream, hlen)) == 0)
                        {
                            rc = hlen;
                            DLOGT("IPPacket::CheckHeader(%x, %d): check header complete, hlen = %d", DLOG_POINTER(stream), size, rc);
                        }
                        else
                        {
                            DLOGI("IPPacket::CheckHeader(%x, %d): bad check sum (%d)", DLOG_POINTER(stream), size, rc);
                        }

                    }
                    else
                    {
                        DLOGI("IPPacket::CheckHeader(%x, %d): bad header length (%d)", DLOG_POINTER(stream), size, len);
                    }
                }
                else
                {
                    DLOGI("IPPacket::CheckHeader(%x, %d): bad STX word ", DLOG_POINTER(stream), size);
                }
            }
            else
            {
                DLOGE("IPPacket::CheckHeader(%x, %d): invalid argument", DLOG_POINTER(stream), size);
            }

            return rc;
        }

        // проверяет полный(!!) IP-пакет по указателю. В случае успеха, возвращает длину пакета в байтах

        int IPPacket::CheckPacket(const void* stream, int size)
        {
            int rc = -EINVAL;

            DLOGT("IPPacket::CheckPacket(%x, %d)", DLOG_POINTER(stream), size);

            if (stream != nullptr && size >= sizeof(headers::IP4HEADER))
            {
                // 1-й этап, проверим заголовок

                rc = CheckHeader(stream, size);

                if (rc > 0)
                {
                    headers::IP4HEADER &ip = *(headers::PIP4HEADER)stream;

                    int len = ::ntohs(ip.length);

                    // 2-й этап, проверяем длину

                    if (len <= size)
                    {
                        rc = len;

                        DLOGI("IPPacket::CheckPacket(%x, %d): chech packet complete, len = %d", DLOG_POINTER(stream), size, len);
                    }
                    else
                    {
                        DLOGE("IPPacket::CheckPacket(%x, %d): bad size, len = %d", DLOG_POINTER(stream), size, len);
                    }
                }
                else
                {
                    DLOGE("IPPacket::CheckPacket(%x, %d): bad header, rc = %d", DLOG_POINTER(stream), size, rc);
                }
            }
            else
            {
                DLOGE("IPPacket::CheckPacket(%x, %d): invalid argument", DLOG_POINTER(stream), size);
            }

            return rc;
        }

        unsigned short IPPacket::CheckSumm(const void* header, int size)
        {

            unsigned int rc = 0;

            unsigned short* p = (unsigned short*)header;

            if (header != nullptr && size > 1)
            {
                rc = 0xFFFF;

                while (size > 1)
                {

                    rc += *p++;

                    size -= sizeof(unsigned short);

                }

                if (size == 1)
                {
                    rc += *(unsigned char*)p;
                }

                rc = ((unsigned short)(rc >> 16) + (rc & 0xFFFF));

                rc += rc >> 16;

                rc ^= 0xFFFF;

            }
            else
            {
                DLOGE("IPPacket::CheckSumm(%x, %d): invalid argument", DLOG_POINTER(header), size);
            }

            return (unsigned short)rc;
        }

        int IPPacket::PutStream(const void* stream, int size, void* context)
        {
            int rc = -EINVAL;

            DLOGT(LOG_BEGIN("PutStream(%x, %d, %x): fb = %d, sb = %d, nb = %d"), DLOG_POINTER(stream), size, DLOG_POINTER(context), frame_bytes_, saved_bytes_, needs_bytes_);

            if (size >= 0 && context != nullptr)
            {

                if (IsComplete())
                {
                    return 0;
                }

                headers::IP4HEADER &ip_header = *(headers::PIP4HEADER)context;

                // смещение задано в октетах

                unsigned short offset = ip_header.off_lo ;

                offset = ip_header.off_hi | (offset << 8);

                // размер заголовка в байтах

                unsigned short hlen = ip_header.hlen << 2;

                // размер pyload'а без заголовка

                unsigned short len = ::ntohs(ip_header.length) - hlen;

                unsigned short pack_id = ::ntohs(ip_header.id);

                unsigned int src_addr = ::ntohl(ip_header.src);

                // ждем новый фрейм

                if (frame_bytes_ < 0)
                {             

                    // защита от дублирования

                    if (!frag_set_.IsSet(offset))
                    {                      

                        // защитим от дублирования

                        frag_set_.Set(offset);

                        offset_ = offset;

                        // если первое вхождение, то сохраним заголовок

                        if (saved_bytes_ == 0)
                        {

                            header_ = ip_header;
                            header_.off_hi = 0;
                            header_.off_lo = 0;
                            header_.length = 0;

                        }

                        // последний пакет (или единственный)

                        if (ip_header.f_M == 0)
                        {

                            // общую длину всех данных вычислим из последнего фрагмента (длина + смещение)

                            needs_bytes_ = len + (offset_ << 3);

                            header_.length = ::htons(needs_bytes_);

                        }

                        // признак начала готовности принимать данные

                        frame_bytes_ = 0;

                        t_stamp_ = tools::Timer::GetTicks();

                        // pack_id_ = pack_id;

                        // src_addr_ = src_addr;

                    }
                    else
                    {
                        // признак дублирования

                        DLOGW(LOG_BEGIN("PutStream(%x, %d, %x): dublicate packet, pack_id =%d, src = %d, offset = %d, len = %d"),
                              DLOG_POINTER(stream),
                              size,
                              DLOG_POINTER(context),
                              pack_id,
                              src_addr,
                              offset,
                              len);

                        rc = -EEXIST;
                    }
                }

                // однозначный ключ принадлежности пакета (pack_id_ и src_addr_ устанавливаются дружественным классом)

                // возможно эту проверку нужно вообще убрать

                if (pack_id_ == pack_id && src_addr_ == src_addr)
                {                    

                    // либо пакет новый, либо не дочитан предыдущий

                    if (frame_bytes_ >= 0)
                    {

                        rc = putData(stream, std::min(size, len - frame_bytes_), offset_ << 3);

                        frame_bytes_ += rc;

                        // общие данные во всех пакетах

                        saved_bytes_ += rc;

                        // если фрагмент прочитан полностью, то сбросим счетчик байт в фрейме

                        if (len == frame_bytes_)
                        {
                            frame_bytes_ = -1;

                            DLOGD(LOG_BEGIN("PutStream(%x, %d, %x): fragment read completed, pack_id =%d, src = %d, offset = %d, len = %d"),
                                  DLOG_POINTER(stream),
                                  size,
                                  DLOG_POINTER(context),
                                  pack_id,
                                  src_addr,
                                  offset,
                                  len);
                        }
                        else
                        {
                            DLOGW(LOG_BEGIN("PutStream(%x, %d, %x): fragment will be read in the next step, pack_id =%d, src = %d, offset = %d, len = %d"),
                                  DLOG_POINTER(stream),
                                  size,
                                  DLOG_POINTER(context),
                                  pack_id,
                                  src_addr,
                                  offset,
                                  len);
                        }

                    }
                }

            }
            else
            {
                DLOGE(LOG_BEGIN("PutStream(%x, %d, %x): invalid argument"), DLOG_POINTER(stream), size, DLOG_POINTER(context));
            }

            return rc;
        }

        int IPPacket::Drop()
        {
            int rc = saved_bytes_;

            DLOGT(LOG_BEGIN("Drop(): fb = %d, sb = %d, nb = %d, pack_id_ = %d, src_addr_ = %d, offset_ = %d"),
                  frame_bytes_,
                  saved_bytes_,
                  needs_bytes_,
                  pack_id_,
                  src_addr_,
                  offset_);

            if (frame_bytes_ > 0)
            {

                saved_bytes_ -= frame_bytes_;
                frag_set_.Clr(offset_);

            }

            frame_bytes_ = -1;

            return rc;
        }

        int IPPacket::Reset()
        {
            int rc = saved_bytes_;

            DLOGT(LOG_BEGIN("Reset(): fb = %d, sb = %d, nb = %d, pack_id_ = %d, src_addr_ = %d, offset_ = %d"),
                  frame_bytes_,
                  saved_bytes_,
                  needs_bytes_,
                  pack_id_,
                  src_addr_,
                  offset_);

            frag_set_.Reset();
            needs_bytes_ = -1;
            frame_bytes_ = -1;
            saved_bytes_ = 0;
            src_addr_ = 0;
            pack_id_ = -1;
            offset_ = 0;

            return rc;
        }

        bool IPPacket::IsComplete() const
        {
            return needs_bytes_ == saved_bytes_;
        }

        bool IPPacket::IsBad() const
        {
            return false;
        }

        /*
        const std::chrono::time_point<std::chrono::system_clock>& IPPacket::GetTimeStamp() const
        {
            return t_stamp_;
        }*/

        const headers::IP4HEADER* IPPacket::Header() const
        {
            return saved_bytes_> 0 ? &header_ : nullptr;
        }

        const void* IPPacket::Pyload() const
        {
            return IsComplete() ? data_.data() : nullptr;
        }

        int IPPacket::Size() const
        {
            return IsComplete() ? saved_bytes_ : 0;
        }

        int IPPacket::putData(const void* stream, int size, int offset)
        {

            DLOGT(LOG_BEGIN("putData(%x, %d, %d)"), DLOG_POINTER(stream), size, offset);

            int total_size = offset + size;

            if (total_size > data_.size())
            {
                DLOGD(LOG_BEGIN("putData(%x, %d, %d): resize data_ %d -> %d"), DLOG_POINTER(stream), size, offset, data_.size(), total_size);

                data_.resize(total_size);
            }

            std::memcpy(data_.data() + offset + frame_bytes_, stream, size);

            return size;
        }
    }
}
