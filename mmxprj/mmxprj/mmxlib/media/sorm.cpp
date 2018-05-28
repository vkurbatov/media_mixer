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
            streams_[0] = nullptr;
            streams_[1] = nullptr;
            rtp_ssrcs_[0] = rtp_ssrcs_[1] = 0;
            rtp_pack_ids_[0] = rtp_pack_ids_[1] = 0;
        }

        Sorm::Sorm(Sorm&& sorm) :
            mixer_gain_(sorm.mixer_gain_),
            media_pool_(sorm.media_pool_),
            sorm_info_(sorm.sorm_info_),
            orm_info_(sorm.orm_info_)
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

                sorm.streams_[0] = nullptr;
                sorm.streams_[1] = nullptr;
                std::memset(&sorm.sorm_info_, 0, sizeof(sorm.sorm_info_));
                std::memset(&sorm.orm_info_, 0, sizeof(sorm.orm_info_));

            }
        }

        Sorm::~Sorm()
        {

            DLOGT(LOG_BEGIN("~Sorm()"));
            Drop();
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

                streams_[1] = (proxy->source_a.address != proxy->source_b.address ||
                        proxy->source_a.port != proxy->source_b.port) ?
                        media_pool_.GetStream(proxy->source_b.address, proxy->source_b.port) :
                            nullptr;


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

                        DLOGT(LOG_BEGIN("OrmInfoPack(): get sample success {i:%d, ssrc:%d, pack_id:%d, size:%d}"),
                              i,
                              rtp_ssrcs_[i],
                              rtp_pack_ids_[i],
                              size_arr[i]);
                    }
                    else
                    {
                        DLOGD(LOG_BEGIN("OrmInfoPack(%x): drop stream %x {i:%d, ssrc:{%d <-> %d}, pack_id:{%d <-> %d}}"),
                              DLOG_POINTER(&writer),
                              DLOG_POINTER(media_samples[i]),
                              i,
                              rtp_ssrcs_[i],
                              media_samples[i]->header.rtp_header.ssrc,
                              pack_id,
                              rtp_pack_ids_[i]);
                    }

                }

            }

            //DLOGT(LOG_BEGIN("OrmInfoPack(): Before writing {%d, %d}"), size_max, rc);

            rc = (int)sizeof(mmx::headers::ORM_INFO_HEADER);

            rc += size_arr[0] + size_arr[1];

            bool f_combined = orm_info_.header.order_header.mcl_b == 0xFF;

            const char* samples[2] = { media_samples[0] != nullptr ? media_samples[0]->media : nullptr,
                                 media_samples[1] != nullptr ? media_samples[1]->media : nullptr
                               };

            int need_size = 160 << (int)(!f_combined);

            bool f_process = true;

            while (f_process)
            {

                f_process = false;

                int ret = fillOrder(samples[0], size_arr[0],
                                        samples[1], size_arr[1],
                                        need_size,
                                        f_combined,
                                        orm_info_
                                );




                if (ret > 0)
                {

                    orm_info_.header.media_size += ret;
                    need_size -= ret;

                    for (int j = 0; j < 2; j++)
                    {
                        if (samples[j] != nullptr && size_arr[j] > 0)
                        {
                            int off = f_combined ? ret : ret / 2;

                            if (off > size_arr[j])
                            {
                                off = size_arr[j];
                            }
                            samples[j] += off;
                            size_arr[j] -= off;
                        }
                    }

                    if (orm_info_.header.media_size > headers::ORDER_645_2_MAX_DATA_SIZE)
                    {
                        DLOGC(LOG_BEGIN("OrmInfoPack(): Syncrinuze error: %d > %d"), orm_info_.header.media_size, headers::ORDER_645_2_MAX_DATA_SIZE);
                        orm_info_.header.media_size = 0;
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

                        writer.Write(&orm_info_, orm_info_.header.media_size + sizeof(headers::ORM_INFO_HEADER));

                        orm_info_.header.media_size = 0;

                    }
                }

            }


            /*

            auto block = writer.QueryBlock(rc);

            if (block != nullptr)
            {

                static unsigned char packet_ids[0x100] = { 0 };

                mmx::headers::ORM_INFO_PACKET& orm_info = *(mmx::headers::ORM_INFO_PACKET*)block->data;

                order_header_.block_number++;
                order_header_.packet_id = packet_ids[sorm_info_.channel_id]++;
                order_header_.conn_flag = conn_flag;    //(int)(size_arr[0] == 0 && size_arr[1] == 0);

                orm_info.header.order_header = order_header_;

                if (order_header_.mcl_a != order_header_.mcl_b)
                {
                    orm_info.header.size_mcl[0] = size_arr[0];
                    orm_info.header.size_mcl[1] = size_arr[1];

                    DLOGT(LOG_BEGIN("OrmInfoPack(): Build separated frame {%d, %d, %d, %d, %d, %d, %d}"),
                          order_header_.block_number,
                          order_header_.packet_id,
                          order_header_.conn_flag,
                          order_header_.mcl_a,
                          order_header_.mcl_b,
                          orm_info.header.size_mcl[0],
                          orm_info.header.size_mcl[1]);

                    for (int j = 0; j < 2; j++)
                    {
                        int i = 0;

                        for (i = 0;i < size_arr[j]; i++)
                        {
                            orm_info.data[(i << 1) + j] = media_samples[j]->media[i];
                        }

                        while (i < size_max)
                        {
                            orm_info.data[(i++ << 1) + j] = headers::ORDER_645_SILENCE_SYMBOL;
                        }
                    }
                }
                else
                {
                    auto size_min = size_arr[0] > size_arr[1] ? size_arr[1] : size_arr[0];


                    orm_info.header.size_mcl[0] = size_max;
                    orm_info.header.size_mcl[1] = 0;
                    orm_info.header.order_header.mcl_b = 0xFF;

                    DLOGT(LOG_BEGIN("OrmInfoPack(): Build mixed frame {%d, %d, %d, %d, %d, %d, %d}"),
                          order_header_.block_number,
                          order_header_.packet_id,
                          order_header_.conn_flag,
                          order_header_.mcl_a,
                          order_header_.mcl_b,
                          orm_info.header.size_mcl[0],
                          orm_info.header.size_mcl[1]);

                    int i = 0;

                    // смешиваем только значимые данные

                    for (;i < size_min; i++)
                    {
                        orm_info.data[i] = codecs::audio::PcmaCodec::Encode(
                            codecs::audio::mixer(
                                codecs::audio::PcmaCodec::Decode(media_samples[0]->media[i]),
                                codecs::audio::PcmaCodec::Decode(media_samples[1]->media[i]),
                                mixer_gain_
                                )
                            );
                    }

                    int j = (int)(size_arr[0] < size_arr[1]);

                    while (i < size_max)
                    {
                        orm_info.data[i] = media_samples[j]->media[i];
                        i++;
                    }
                }

                writer.Commit();
            }
            else
            {
                DLOGW(LOG_BEGIN("OrmInfoPack(&%x): Can't get block size = %d"), DLOG_POINTER(&writer), rc);
                rc = -ENOMEM;
            }

            */

            return rc;
        }

        void Sorm::Drop()
        {
            // пока не знаю что сюда написать
        }

        void Sorm::setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm)
        {
            if (&sorm != &sorm_info_)
            {
                sorm_info_ = sorm;
            }
            orm_info_.header.media_size = 0;
            orm_info_.header.order_header.magic = headers::ORDER_645_2_MAGIC;
            orm_info_.header.order_header.block_number = 0;
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


        int Sorm::fillOrder(const void *data_a, int size_a, const void *data_b, int size_b, int need_size, bool combined, headers::ORM_INFO_PACKET &orm_info)
        {
            int rc = 0;

            int offset = orm_info.header.media_size;

            if ((offset + need_size) > headers::ORDER_645_2_MAX_DATA_SIZE)
            {
                need_size = headers::ORDER_645_2_MAX_DATA_SIZE - offset;
            }

            rc = need_size;

            if (!combined)
            {
                need_size /= 2;
            }

            if (size_a > need_size)
            {
                size_a = need_size;
            }
            if (size_b > need_size)
            {
                size_b = need_size;
            }

            auto p = &media_pool_;

            int min_size = (size_a > size_b) ? size_b : size_a;
            int max_size = (size_a < size_b) ? size_b : size_a;

            if (need_size == 64)
            {
                need_size = 64;
            }

            int i = 0;
            int idx = 0;

            if (!combined)
            {

                // mcl_a
                for (;i < size_a; i++)
                {
                    idx = offset + (i << 1);
                    orm_info.data[idx] = ((char*)data_a)[i];
                }

                while (i < (need_size))
                {
                    idx = offset + (i++ << 1);
                    orm_info.data[idx] = headers::ORDER_645_SILENCE_SYMBOL;

                }

                // mcl_b
                i = 0;
                for (;i < size_b; i++)
                {
                    idx = offset + (i << 1) + 1;
                    orm_info.data[idx] =((char*)data_b)[i];
                }

                while (i < (need_size))
                {
                    idx = offset + (i++ << 1) + 1;
                    orm_info.data[idx] = headers::ORDER_645_SILENCE_SYMBOL;

                }
            }
            else
            {
                int i = 0;

                for (;i < min_size; i++)
                {
                    orm_info.data[offset + i] = codecs::audio::PcmaCodec::Encode(
                        codecs::audio::mixer(
                            codecs::audio::PcmaCodec::Decode(((unsigned char*)data_a)[i]),
                            codecs::audio::PcmaCodec::Decode(((unsigned char*)data_b)[i]),
                            mixer_gain_
                            )
                        );
                }

                const void* media_data = size_a < size_b ? data_b : data_a;

                while (i < max_size)
                {
                    orm_info.data[offset + i] = ((unsigned char*)media_data)[i];
                    i++;
                }

                while (i < (need_size))
                {
                    orm_info.data[offset + i] = headers::ORDER_645_SILENCE_SYMBOL;
                    i++;
                }
            }

            if (p != &media_pool_)
            {
                rc = 0;
            }

            return rc;

        }
    }
}
