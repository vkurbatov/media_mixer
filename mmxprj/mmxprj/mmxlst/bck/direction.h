#ifndef _MMXLST_DIRECTION_H
#define _MMXLST_DIRECTION_H

#include <vector>

#include "mmxlib/ipc/ichannel.h"
#include "mmxlib/data/datapacket.h"
#include "mmxlib/net/portset.h"
#include "mmxlib/headers/ip.h"



namespace mmxlst
{
    class Direction : public mmx::staff::IStaffer
    {
        mmx::net::PortSet& ports_;
        // std::vector<char> data_;

        mmx::staff::DataPacket& data_;


    public:
        Direction(mmx::staff::DataPacket& datapack, mmx::net::PortSet& ports);
        ~Direction();

        virtual int Dispatch(const char* stream, int size, mmx::staff::PPROTOCOL_INFO pinfo = nullptr);
        virtual void Reset();

        int Sniffer(const char* pyload, int size, const mmx::headers::IP4HEADER& ip_header);

        //mmx::headers::PDATA_PACK GetPacket();
        //void Next();
        //bool IsEmpty() const;

    };
}

#endif

