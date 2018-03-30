#ifndef _MMX_STAFF_IP_STAFFER_H
#define _MMX_STAFF_IP_STAFFER_H

#include "istaffer.h"
#include "headers/ip.h"

namespace mmx
{
    namespace staff
    {
        class IPStaffer : public IStaffer
        {
            IStaffer*           next_;
            int                 pos_;
            headers::IP4HEADER  ip4_header_;
        public:
            IPStaffer(IStaffer* next = nullptr);
            ~IPStaffer() override;
            int Dispatch(const char* stream, int size, PPROTOCOL_INFO pinfo = nullptr) override;
            void Reset() override;
        };
    }
}

#endif
