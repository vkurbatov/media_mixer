#ifndef _MMX_SNIFFERS_DP_SNIFFER_H
#define _MMX_SNIFFERS_DP_SNIFFER_H

//#include <vector>

#include "istream.h"
#include "headers/datapack.h"

namespace mmx
{
    namespace sniffers
    {
        class DataPackSniffer : public IStream
        {
            enum sniffer_state_t
            {
                SS_START1,
                SS_START2,
                SS_HEADER,
                SS_PYLOAD,
                SS_STOP1,
                SS_STOP2,
                SS_NEXT,
                SS_BAD
            }state_;

            int saved_bytes_;

            union
            {
                headers::DATA_PACK  data_pack_;
                char raw_data[headers::MAX_PACKET_SIZE + 1];
            };

            headers::PDATA_PACK work_data_pack_;

        public:
            DataPackSniffer();
            DataPackSniffer(DataPackSniffer&& sniffer);


            // IStream
            int PutStream(const void* stream, int size, void* hcontext = nullptr) override;
            int Drop() override;
            int Reset() override;
            bool IsComplete() const override;
            bool IsBad() const override;

            const headers::DATA_PACK* GetDataPacket() const;

        private:

            int forceSniffer(const void* stream, int size);
            int fragmentSniffer(const void* stream, int size);

        };
    }
}

#endif
