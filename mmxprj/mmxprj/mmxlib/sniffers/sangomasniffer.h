#ifndef _MMX_SNIFFERS_SANGOMA_SNIFFER_H
#define _MMX_SNIFFERS_SANGOMA_SNIFFER_H

#include "istream.h"
#include "headers/si.h"

namespace mmx
{
    namespace sniffers
    {
        class SangomaSniffer : public IStream
        {

            enum sniffer_state_t
            {
                SS_START,
                SS_HEADER,
                SS_PYLOAD,
                SS_NEXT,
                SS_BAD
            }state_;

            headers::SANGOMA_MEDIA_STREAM_PACKET* sangoma_;

            union
            {
                headers::SANGOMA_MEDIA_STREAM_PACKET  sangoma_pack_;
                char raw_data[sizeof(headers::SANGOMA_MEDIA_STREAM_PACKET)];
            };

            int     saved_bytes_;

        public:
            SangomaSniffer();
            ~SangomaSniffer() override;
            // IStream
            int PutStream(const void* stream, int size, void* hcontext = nullptr) override;
            int Drop() override;
            int Reset() override;
            bool IsComplete() const override;
            bool IsBad() const override;

            const headers::SANGOMA_MEDIA_STREAM_PACKET* GetPacket();

        private:

            int forceSniffer(const void* stream, int size);
            int fragmentSniffer(const void* stream, int size);

        };
    }
}

#endif
