#ifndef _MMX_SNIFFERS_IP_SNIFFER_H
#define _MMX_SNIFFERS_IP_SNIFFER_H

#include "istream.h"
#include "iippacket.h"
#include "ippacketwrapper.h"
#include "ippacketpool.h"

namespace mmx
{
    namespace sniffers
    {

        static const int MAX_IP_HEADER_SIZE = 60;

        class IPSniffer : public IStream
        {
            enum sniffer_state_t
            {
                SS_START,
                SS_HEADER,
                SS_PYLOAD,
                SS_NEXT,
                SS_BAD
            }state_;

            IPPacketPool&       packet_pool_;
            IPPacket*           packet_;
            int                 saved_bytes_;

            union
            {
                headers::IP4HEADER  header_;
                char                raw_header_[headers::MAX_HEADER_LEN * 4];
            };

            IPPacketWrapper     wrapper_;

        public:

            IPSniffer(IPPacketPool& packet_pool);

            // IStream

            int PutStream(const void* stream, int size, void* context = nullptr) override;
            int Drop() override;
            int Reset() override;
            bool IsComplete() const override;
            bool IsBad() const override;

            const IIPPacket* GetPacket() const;

        private:
            int forceSniffer(const void* stream, int size);
            int fragmentSniffer(const void* stream, int size);

        };
    }
}

#endif

