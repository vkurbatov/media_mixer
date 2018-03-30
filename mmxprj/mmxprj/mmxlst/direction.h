#ifndef _MMXLST_DIRECTION_H
#define _MMXLST_DIRECTION_H

#include <vector>

#include "mmxlib/staff/istaffer.h"
#include "mmxlib/ipc/ichannel.h"
#include "mmxlib/staff/datapacket.h"
#include "mmxlib/net/portset.h"



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

        //mmx::headers::PDATA_PACK GetPacket();
        //void Next();
        //bool IsEmpty() const;

    };
}

#endif

