#include "sorm.h"
#include "media/codecs/audio/mixer.h"
#include "media/codecs/audio/pcma.h"

#include <cstring>
#include <netdb.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("Sorm", msg)

namespace mmx
{

    namespace media
    {

        Sorm::Sorm(MediaPool& media_pool, unsigned char mixer_gain) :
            mixer_gain_(mixer_gain),
            media_pool_(media_pool)

        {

            DLOGT(LOG_BEGIN("Sorm(%d)"), mixer_gain);

            std::memset(&sorm_info_, 0, sizeof(sorm_info_));
            std::memset(&orm_info_, 0, sizeof(orm_info_));
            std::memset(&io_info_, 0, sizeof(io_info_));

            streams_[0] = nullptr;
            streams_[1] = nullptr;
            rtp_ssrcs_[0] = rtp_ssrcs_[1] = 0;
            rtp_pack_ids_[0] = rtp_pack_ids_[1] = 0;
        }

        Sorm::Sorm(Sorm&& sorm) :
            mixer_gain_(sorm.mixer_gain_),
            media_pool_(sorm.media_pool_),
            sorm_info_(sorm.sorm_info_),
            orm_info_(sorm.orm_info_),
            io_info_(sorm.io_info_)
        {

            DLOGT(LOG_BEGIN("Sorm(&&%x)"), DLOG_POINTER(&sorm));

            streams_[0] = sorm.streams_[0];
            streams_[1] = sorm.streams_[1];
            rtp_ssrcs_[0] = sorm.rtp_ssrcs_[0] ;
            rtp_ssrcs_[1] = sorm.rtp_ssrcs_[1];
            rtp_pack_ids_[0] = sorm.rtp_pack_ids_[0];
            rtp_pack_ids_[1] = sorm.rtp_pack_ids_[1];

            sorm.streams_[0] = nullptr;
            sorm.streams_[1] = nullptr;

            std::memset(&sorm.sorm_info_, 0, sizeof(sorm.sorm_info_));
            std::memset(&sorm.orm_info_, 0, sizeof(sorm.orm_info_));
            std::memset(&sorm.io_info_, 0, sizeof(sorm.io_info_));

        }

        Sorm& Sorm::operator=(Sorm&& sorm)
        {
            if (this != &sorm)
            {
                DLOGT(LOG_BEGIN("operator=(&&%x)"), DLOG_POINTER(&sorm));

                mixer_gain_ = sorm.mixer_gain_;
                //media_pool_ = channel.media_pool_;
                streams_[0] = sorm.streams_[0];
                streams_[1] = sorm.streams_[1];
                rtp_ssrcs_[0] = sorm.rtp_ssrcs_[0] ;
                rtp_ssrcs_[1] = sorm.rtp_ssrcs_[1];
                rtp_pack_ids_[0] = sorm.rtp_pack_ids_[0];
                rtp_pack_ids_[1] = sorm.rtp_pack_ids_[1];


                sorm_info_ = sorm.sorm_info_;
                orm_info_ = sorm.orm_info_;
                io_info_ = sorm.io_info_;

                sorm.streams_[0] = nullptr;
                sorm.streams_[1] = nullptr;
                std::memset(&sorm.sorm_info_, 0, sizeof(sorm.sorm_info_));
                std::memset(&sorm.orm_info_, 0, sizeof(sorm.orm_info_));
                std::memset(&sorm.io_info_, 0, sizeof(sorm.io_info_));

            }
        }

        Sorm::~Sorm()
        {

            DLOGT(LOG_BEGIN("~Sorm()"));
            Reset();
            SetProxy();

        }

        int Sorm::SetProxy(const mmx::headers::SANGOMA_PROXY_INFO* proxy)
        {

            int rc = 0;

            DLOGT(LOG_BEGIN("SetProxy(%x)"), DLOG_POINTER(proxy));

            // сперва освободим имеющиеся медиапотоки

            for (auto& s : streams_)
            {
                if (s != nullptr)
                {                   
                    media_pool_.Release(s);                    
                    s = nullptr;
                }
            }
            // если проксирование не задано, то вызов был для освобождения ресурсов

            if (proxy != nullptr)
            {
                DLOGT(LOG_BEGIN("SetProxy(): new binding"));

                streams_[0] = media_pool_.GetStream(proxy->source_a.address, proxy->source_a.port);

                // если приходит совмещенный контроль, то медиапотоки идентичны

                streams_[1] = media_pool_.GetStream(proxy->source_b.address, proxy->source_b.port);


                rc = (int)(streams_[0] != nullptr) + (int)(streams_[1] != nullptr);
            }

            return rc;
        }

        const mmx::headers::SANGOMA_SORM_INFO& Sorm::GetOrmInfo() const
        {
            return sorm_info_;
        }

        int Sorm::OrmInfoPack(data::IDataPacketWriter& writer, unsigned char conn_flag)
        {
            int rc = 0;

            const mmx::headers::MEDIA_SAMPLE* media_samples[STREAM_COUNT] = { nullptr };

            unsigned short size_arr[STREAM_COUNT] = { 0 };

            DLOGT(LOG_BEGIN("OrmInfoPack(%x)"), DLOG_POINTER(&writer));

            for (int i = 0; i < STREAM_COUNT; i++)
            {

                media_samples[i] = (streams_[i] != nullptr) ?
                            streams_[i]->GetMediaSample() : nullptr;

                if (media_samples[i] != nullptr)
                {

                    unsigned short pack_id = ::ntohs(media_samples[i]->header.rtp_header.packet_id);

                    short delta = pack_id - rtp_pack_ids_[i];

                    if (rtp_ssrcs_[i] == 0 || rtp_ssrcs_[i] != media_samples[i]->header.rtp_header.ssrc ||
                        delta > 0)
                    {

                        rtp_ssrcs_[i] = media_samples[i]->header.rtp_header.ssrc;
                        rtp_pack_ids_[i] = pack_id;// media_samples[i]->header.rtp_header.packet_id;

                        size_arr[i] = media_samples[i]->header.length - sizeof(media_samples[i]->header);



                        DLOGT(LOG_BEGIN("OrmInfoPack(): get sample success {i:%d, ssrc:%x, pack_id:%x, size:%d}"),
                              i,
                              rtp_ssrcs_[i],
                              rtp_pack_ids_[i],
                              size_arr[i]);
                    }
                    else
                    {
                        DLOGD(LOG_BEGIN("OrmInfoPack(%x): drop stream %x {i:%d, ssrc:{%x <-> %x}, pack_id:{%d <-> %d}}, delta = %d"),
                              DLOG_POINTER(&writer),
                              DLOG_POINTER(media_samples[i]),
                              i,
                              rtp_ssrcs_[i],
                              media_samples[i]->header.rtp_header.ssrc,
                              pack_id,
                              rtp_pack_ids_[i],
                              delta);
                        media_samples[i] = nullptr;
                    }

                }

            }

            //DLOGT(LOG_BEGIN("OrmInfoPack(): Before writing {%d, %d}"), size_max, rc);

            rc = (int)sizeof(mmx::headers::ORM_INFO_HEADER);

            fillOrder(writer, media_samples, conn_flag);

            return rc;
        }

        int Sorm::Reset()
        {
            orm_info_.header.order_header.block_number = 0;
            std::memset(&io_info_,0, sizeof(io_info_));
        }

        const Sorm::io_info_t& Sorm::GetDiagInfo() const
        {
            return io_info_;
        }

        void Sorm::setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm)
        {
            if (&sorm != &sorm_info_)
            {
                sorm_info_ = sorm;
            }
            orm_info_.header.media_size = 0;
            orm_info_.header.order_header.magic = headers::ORDER_645_2_MAGIC;
            orm_info_.header.order_header.sorm_id = sorm.sorm_id;
            orm_info_.header.order_header.object_id = sorm.object_id;
            orm_info_.header.order_header.call_id = sorm.call_id;
            orm_info_.header.order_header.mcl_a = sorm.mcl_a;
            orm_info_.header.order_header.mcl_b = sorm.mcl_a != sorm.mcl_b ? sorm.mcl_b : 0xFF;
            orm_info_.header.order_header.conn_param = sorm.conn_param;

            DLOGT(LOG_BEGIN("setSorm(&%x): {%d, %d, %d, %d, %d, %d, %d, %d, %d}"), DLOG_POINTER(&sorm),
                  orm_info_.header.order_header.sorm_id,
                  orm_info_.header.order_header.object_id,
                  orm_info_.header.order_header.call_id,
                  orm_info_.header.order_header.mcl_a,
                  orm_info_.header.order_header.mcl_b,
                  orm_info_.header.order_header.conn_param
                  );

        }

        int Sorm::fillOrder(data::IDataPacketWriter& writer, const mmx::headers::MEDIA_SAMPLE* media_samples[], unsigned char conn_flag)
        {
            int rc = 0;

            bool f_process = true;

            const char* media[2] = { nullptr, nullptr};
            int size_arr[2] = { 0, 0 };

            for (int j = 0; j < STREAM_COUNT; j++)
            {
                if (media_samples[j] != nullptr)
                {
                    media[j] = media_samples[j]->media;
                    size_arr[j] = (media_samples[j]->header.length - sizeof(media_samples[j]->header));

                    io_info_.rtp_packs[j]++;
                    io_info_.rtp_bytes[j] += size_arr[j];
                }
            }

/*
            char media_a[160];
            char media_b[160];

            int size_arr[] = { 160, 160 };
            const char* media[2] = { media_a , media_b };

            for (int i = 0; i < 160; i++)
            {
                media_a[i] = dummed;
                media_b[i] = dummed++;
            }
*/
            bool combined = orm_info_.header.order_header.mcl_b == 0xFF;

            int need_size = 160 << (!combined);

            while (f_process)
            {

                f_process = false;

                int ret = combined
                        ? pushCombineMedia(media[0], size_arr[0],
                                        media[1], size_arr[1],
                                        need_size, orm_info_.data + orm_info_.header.media_size,
                                        headers::ORDER_645_2_MAX_DATA_SIZE - orm_info_.header.media_size,
                                        mixer_gain_,
                                        io_info_.order645_bytes)
                        : pushSeparatedMedia(media[0], size_arr[0],
                                             media[1], size_arr[1],
                                             need_size,
                                             orm_info_.data + orm_info_.header.media_size,
                                             headers::ORDER_645_2_MAX_DATA_SIZE - orm_info_.header.media_size,
                                             io_info_.order645_bytes);

                if (ret > 0)
                {

                    rc += ret;
                    orm_info_.header.media_size += ret;
                    need_size -= ret;

                    for (int j = 0; j < 2; j++)
                    {
                        if (media[j] != nullptr && size_arr[j] > 0)
                        {
                            int off = combined ? ret : ret / 2;

                            if (off > size_arr[j])
                            {
                                off = size_arr[j];
                            }
                            media[j] += off;
                            size_arr[j] -= off;
                        }
                    }

                    if (orm_info_.header.media_size > headers::ORDER_645_2_MAX_DATA_SIZE)
                    {
                        DLOGC(LOG_BEGIN("fillOrder(): Syncrinuze error: %d > %d"), orm_info_.header.media_size, headers::ORDER_645_2_MAX_DATA_SIZE);
                        orm_info_.header.media_size = 0;
                        rc = -EBADMSG;
                        break;

                    }

                    if ((orm_info_.header.media_size == headers::ORDER_645_2_MAX_DATA_SIZE) || (conn_flag != 0))
                    {
                        static unsigned char packet_ids[0x100] = { 0 };

                        orm_info_.header.order_header.block_number++;
                        orm_info_.header.order_header.packet_id = packet_ids[sorm_info_.channel_id]++;
                        orm_info_.header.order_header.conn_flag = conn_flag;

                        if (need_size > 0 && conn_flag == 0)
                        {
                            f_process = true;
                        }

                        io_info_.order645_packs ++;
                        io_info_.order645_bytes += orm_info_.header.media_size;

                        writer.Write(&orm_info_, orm_info_.header.media_size + sizeof(headers::ORM_INFO_HEADER));

                        orm_info_.header.media_size = 0;

                    }
                }

            }


            return rc;
        }

        int Sorm::pushCombineMedia(const void *data_a, int size_a, const void *data_b, int size_b, int need_size, void* out_data, int out_size, int mixer_gain, int& count)
        {

            int rc = need_size = need_size > out_size ?
                     out_size :
                     need_size;


            size_a = std::min(size_a, need_size);
            size_b = std::min(size_b, need_size);
            int size_min = std::min(size_a, size_b);
            int size_max = std::max(size_a, size_b);


            int i = 0;

            if (data_a != nullptr && data_b != nullptr)
            {

                if (data_a == data_b)
                {
                    size_min = 0;
                }

                for(; i < size_min; i++)
                {
                    ((char*)out_data)[i] = codecs::audio::PcmaCodec::Encode(
                        codecs::audio::mixer(
                            codecs::audio::PcmaCodec::Decode(((unsigned char*)data_a)[i]),
                            codecs::audio::PcmaCodec::Decode(((unsigned char*)data_b)[i]),
                            mixer_gain
                            )
                        );
                }

                count += size_min * 2;

                std::memcpy((char*)out_data + i, data_a == nullptr ? data_b : data_a, size_max - size_min);
                i += size_max - size_min;
                count += size_max - size_min;
            }

            std::memset((char*)out_data + i, headers::ORDER_645_SILENCE_SYMBOL, need_size - i);

            //int s = idx + i + need_size - i;

            return rc;
        }

        int Sorm::pushSeparatedMedia(const void *data_a, int size_a, const void *data_b, int size_b, int need_size, void* out_data, int out_size, int& count)
        {

            int rc = need_size = need_size > out_size ?
                     out_size :
                     need_size;

            need_size /= 2;

            size_a = std::min(size_a, need_size);
            size_b = std::min(size_b, need_size);

            struct
            {
                const char* data;
                int size;
            }samples[] = {
            {(const char*) data_a, size_a},
            {(const char*) data_b, size_b},
            };

            for (int j = 0; j < STREAM_COUNT; j++)
            {
                int i = 0;

                count += samples[j].size;

                for (;i < samples[j].size; i++)
                {
                    ((char*)out_data)[(i << 1) + j] = (samples[j].data)[i];
                }

                while (i < need_size)
                {
                    ((char*)out_data)[(i << 1) + j] = headers::ORDER_645_SILENCE_SYMBOL;
                    i++;
                }
            }

            return rc;
        }

    }

}
