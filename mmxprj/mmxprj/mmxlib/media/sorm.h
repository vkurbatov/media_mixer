#ifndef _MMX_MEDIA_SORM_H
#define _MMX_MEDIA_SORM_H

#include "mediapool.h"

#include "headers/si.h"
#include "headers/media.h"
#include "headers/order645_2.h"
#include "data/idpwriter.h"

namespace mmx
{
    namespace media
    {


        class SormPool;
        class Sorm
        {

            const static int STREAM_COUNT = 2;
            const static int DEFAULT_MIXER_GAIN = 50;

            //unsigned char dummed = 0;

            mmx::headers::SANGOMA_SORM_INFO sorm_info_;

            //mmx::headers::ORDER_645_2_HEADER order_header_;
            union
            {
                mmx::headers::ORM_INFO_PACKET orm_info_;
                unsigned char orm_info_raw_[headers::ORDER_645_2_TOTAL_PACKET_SIZE + 2];
            };

            const MediaStream* streams_[STREAM_COUNT];

            unsigned int        rtp_ssrcs_[STREAM_COUNT];
            unsigned int        rtp_pack_ids_[STREAM_COUNT];

            unsigned int        last_update_media_;
            unsigned char       mixer_gain_;

            MediaPool&          media_pool_;




            friend class SormPool;
            Sorm(MediaPool& media_pool, unsigned char mixer_gain = DEFAULT_MIXER_GAIN);
        public:

            struct io_info_t
            {
                int rtp_packs[2];
                int rtp_bytes[2];
                int order645_packs;
                int order645_bytes;
            };

            ~Sorm();
            Sorm(Sorm&& channel);
            Sorm& operator=(Sorm&& channel);
            int SetProxy(const mmx::headers::SANGOMA_PROXY_INFO* proxy = nullptr);
            const mmx::headers::SANGOMA_SORM_INFO& GetOrmInfo() const;
            int ProcessMediaStreams(data::IDataPacketWriter& writer, unsigned char conn_flag = 0);
            int PutPreamble(data::IDataPacketWriter& writer, int packet_count);
            int Reset();

            unsigned int GetMediaDelay() const;

            const io_info_t& GetDiagInfo() const;


        private:

            io_info_t io_info_;
            int putPack(data::IDataPacketWriter& writer);
            void setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm);
            //int fillOrder(const void *data_a, int size_a, const void *data_b, int size_b, int need_size, bool combined, headers::ORM_INFO_PACKET &orm_info);
            int fillOrder(data::IDataPacketWriter& writer, const mmx::headers::MEDIA_SAMPLE* media_samples[], unsigned char conn_flag);
            static int pushCombineMedia(const void *data_a, int size_a, const void *data_b, int size_b, int need_size, void* out_data, int out_size, int mixer_gain, int& count);
            static int pushSeparatedMedia(const void *data_a, int size_a, const void *data_b, int size_b, int need_size, void* out_data, int out_size, int& count);

        };
    }
}

#endif
