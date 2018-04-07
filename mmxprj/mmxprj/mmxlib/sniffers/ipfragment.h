#ifndef _MMX_SNIFFERS_IP_FRAGMENT_H
#define _MMX_SNIFFERS_IP_FRAGMENT_H

#include <vector>
#include "ippacket.h"

namespace mmx
{
    namespace sniffers
    {

        static const int DEFAULT_MTU_SIZE = 1600;

        class IPFragment
        {
            IPPacket            packet_;
            std::vector<char>   data_;
            int                 total_bytes_;
            int                 saved_bytes_;
            bool                fragment_;
            bool                last_packet_;

        public:
            IPFragment();
            IPFragment(IPFragment&& fragment);
            int Sniffer(void* stream, int size);
            void Reset();
            bool IsComplete() const;
        private:
            int readFragment(void* stream, int size);

        };
    }
}

#endif

