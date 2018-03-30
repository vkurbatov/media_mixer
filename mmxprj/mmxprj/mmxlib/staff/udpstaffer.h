#ifndef _MMX_STAFF_UDP_STAFFER_H
#define _MMX_STAFF_UDP_STAFFER_H

#include "istaffer.h"
#include "../headers/udp.h"

namespace mmx
{
    namespace staff
    {
        class UDPStaffer : public IStaffer
        {
            IStaffer*           next_;
            int                 pos_;
        public:
            UDPStaffer(IStaffer* next = nullptr);
            ~UDPStaffer() override;
            int Dispatch(const char* stream, int size, PPROTOCOL_INFO pinfo = nullptr) override;
            void Reset() override;
        };
    }
}

#endif
