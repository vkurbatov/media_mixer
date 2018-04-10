#include "direction.h"
#include "mmxlib/headers/media.h"
#include "mmxlib/headers/udp.h"

#include <netdb.h>      // htons/htonl
#include <sys/time.h>   // gettimeofday

#include <iostream>

namespace mmxlst
{

    Direction::Direction(mmx::staff::DataPacket& datapack, mmx::net::PortSet& ports) :
        data_(datapack),
        ports_(ports)
    {

    }

    Direction::~Direction()
    {

    }

    int Direction::Dispatch(const char* stream, int size, mmx::staff::PPROTOCOL_INFO pinfo)
    {
        int rc = 0;

        // это наш производный класс, и мы знаем что структура pinfo передается всегда

        int len = size + sizeof(mmx::headers::MEDIA_HEADER);


        mmx::headers::UDPIP4& udpip = *(mmx::headers::PUDPIP4)pinfo->parent->header;

        unsigned short port_dst = ::htons(udpip.udp_header.port_dst);

        // условие попадания портов в диапазон

        if (ports_[port_dst])
        {
            /*
            if (need_init_ == true)
            {
                data_.Init(++pack_id_);
                need_init_ = false;
            }
            */

            auto block = data_.QueryData(size + sizeof(mmx::headers::MEDIA_HEADER));

            if(block != nullptr)
            {
                mmx::headers::MEDIA_DATA& media =  *(mmx::headers::PMEDIA_DATA)block->data;

                media.header.magic = mmx::headers::MEDIA_MAGIC;
                media.header.length = size + sizeof(mmx::headers::MEDIA_HEADER);
                media.header.addr_src = ::htonl(udpip.ip4_header.src);
                media.header.addr_dst = ::htonl(udpip.ip4_header.dest);
                media.header.port_src = ::htons(udpip.udp_header.port_src);
                media.header.port_dst = port_dst;

                timeval tv;
                gettimeofday(&tv, 0);

                media.header.sec = tv.tv_sec;
                media.header.usec = tv.tv_usec;

                std::copy(stream, stream + size, media.media);

                rc = size;

            }

        }

        return rc;

    }

    int Direction::Sniffer(const char* pyload, int size, const mmx::headers::IP4HEADER& ip_header)
    {
        int rc = -EINVAL;

        if (pyload != nullptr && size >= sizeof(mmx::headers::UDPHEADER))
        {
            rc = -EPROTO;

            if (ip_header.protocol == IPPROTO_UDP)
            {
                mmx::headers::UDPHEADER& udp = *(mmx::headers::PUDPHEADER)pyload;

                unsigned short port_dst = ::htons(udp.port_dst);

                rc = -EBADMSG;

                if (::ntohs(udp.length) == size)
                {
                    pyload += sizeof(mmx::headers::UDPHEADER);
                    size -= sizeof(mmx::headers::UDPHEADER);

                    rc= -EADDRNOTAVAIL;

                    if (ports_[port_dst])
                    {
                        rc = -ENOMEM;

                        auto block = data_.QueryData(size + sizeof(mmx::headers::MEDIA_HEADER));

                        if(block != nullptr)
                        {
                            mmx::headers::MEDIA_DATA& media =  *(mmx::headers::PMEDIA_DATA)block->data;

                            media.header.magic = mmx::headers::MEDIA_MAGIC;
                            media.header.length = size + sizeof(mmx::headers::MEDIA_HEADER);
                            media.header.addr_src = ::htonl(ip_header.src);
                            media.header.addr_dst = ::htonl(ip_header.dest);
                            media.header.port_src = ::htons(udp.port_src);
                            media.header.port_dst = port_dst;

                            timeval tv;

                            gettimeofday(&tv, 0);

                            media.header.sec = tv.tv_sec;
                            media.header.usec = tv.tv_usec;

                            std::copy(pyload, pyload + size, media.media);

                            rc = size;

                        }
                    }

                }

            }
        }

        return rc;
    }

    void Direction::Reset()
    {
        // пока не реализовано
    }
/*
    mmx::headers::PDATA_PACK Direction::GetPacket()
    {

        if (need_init_ == true)
        {
            return nullptr;
        }
        else
        {
            return (mmx::headers::PDATA_PACK)data_;
        }
    }*/
/*
    void Direction::Next()
    {
        need_init_ = true;
    }

    bool Direction::IsEmpty() const
    {
        return need_init_;
    }
*/
}
