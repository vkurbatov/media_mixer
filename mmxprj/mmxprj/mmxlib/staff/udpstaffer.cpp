#include "udpstaffer.h"
#include <cstring>

#include <errno.h>
#include <netdb.h>

namespace mmx
{
    namespace staff
    {
        UDPStaffer::UDPStaffer(IStaffer* next) :
            next_(next), pos_(0)
        {

        }

        UDPStaffer::~UDPStaffer()
        {

        }

        int UDPStaffer::Dispatch(const char* stream, int size, PPROTOCOL_INFO pinfo)
        {

            int rc = -EINVAL;

            if (stream != nullptr && size > 0)
            {

                headers::UDPHEADER &udp_header = *(headers::UDPHEADER*)stream;
                unsigned short len = (udp_header.length << 8) | (udp_header.length >> 8);

                PROTOCOL_INFO pi = { pinfo, stream, IPPROTO_UDP };

                if (next_ != nullptr)
                {
                    rc = next_->Dispatch(stream + sizeof(udp_header), len - sizeof(udp_header), &pi);
                }
                else
                {
                    rc = size;
                }

            }

            return rc;
        }

        void UDPStaffer::Reset()
        {
            pos_ = 0;
        }
    }
}
