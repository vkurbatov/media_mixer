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
            std::memset(&order_header_, 0, sizeof(order_header_));
            streams_[0] = nullptr;
            streams_[1] = nullptr;
            rtp_ssrcs_[0] = rtp_ssrcs_[1] = 0;
            rtp_pack_ids_[0] = rtp_pack_ids_[1] = 0;
        }

        Sorm::Sorm(Sorm&& sorm) :
            mixer_gain_(sorm.mixer_gain_),
            media_pool_(sorm.media_pool_),
            sorm_info_(sorm.sorm_info_),
            order_header_(sorm.order_header_)
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

            std::memset(&sorm_info_, 0, sizeof(sorm_info_));
            std::memset(&order_header_, 0, sizeof(order_header_));

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
                order_header_ = sorm.order_header_;

                sorm.streams_[0] = nullptr;
                sorm.streams_[1] = nullptr;
                std::memset(&sorm_info_, 0, sizeof(sorm_info_));
                std::memset(&order_header_, 0, sizeof(order_header_));

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
                streams_[1] = media_pool_.GetStream(proxy->source_b.address, proxy->source_b.port);

                rc = (int)(streams_[0] != nullptr) + (streams_[1] != nullptr);
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

            auto size_max = size_arr[0] > size_arr[1] ? size_arr[0] : size_arr[1];

            rc = (int)sizeof(mmx::headers::ORM_INFO_HEADER);
            rc += order_header_.mcl_a == order_header_.mcl_b ? size_max : size_arr[0] + size_arr[1];

            DLOGT(LOG_BEGIN("OrmInfoPack(): Before writing {%d, %d}"), size_max, rc);

            auto block = writer.QueryBlock(rc);

            if (block != nullptr)
            {

                static unsigned char packet_ids[0x100] = { 0 };

                mmx::headers::ORM_INFO_PACKET& orm_info = *(mmx::headers::ORM_INFO_PACKET*)block->data;



                order_header_.block_number++;
                order_header_.packet_id = packet_ids[sorm_info_.channel_id]++;
                order_header_.conn_flag = conn_flag;//(int)(size_arr[0] == 0 && size_arr[1] == 0);

                orm_info.header.order_header = order_header_;


                if (order_header_.mcl_a != order_header_.mcl_b)
                {
                    orm_info.header.size_a = size_arr[0];
                    orm_info.header.size_b = size_arr[1];

                    DLOGT(LOG_BEGIN("OrmInfoPack(): Build separated frame {%d, %d, %d, %d, %d, %d, %d}"),
                          order_header_.block_number,
                          order_header_.packet_id,
                          order_header_.conn_flag,
                          order_header_.mcl_a,
                          order_header_.mcl_b,
                          orm_info.header.size_a,
                          orm_info.header.size_b);

                    for (int j = 0; j < 2; j++)
                    {
                        int i = 0;

                        for (i = 0;i < size_arr[j]; i++)
                        {
                            orm_info.data[(i << 1) + j] = media_samples[j]->media[i];
                        }

                        while (i < size_max)
                        {
                            orm_info.data[(i++ << 1) + j] = 0x7E;
                        }
                    }
                }
                else
                {
                    auto size_min = size_arr[0] > size_arr[1] ? size_arr[1] : size_arr[0];


                    orm_info.header.size_a = size_max;
                    orm_info.header.size_b = 0;

                    DLOGT(LOG_BEGIN("OrmInfoPack(): Build mixed frame {%d, %d, %d, %d, %d, %d, %d}"),
                          order_header_.block_number,
                          order_header_.packet_id,
                          order_header_.conn_flag,
                          order_header_.mcl_a,
                          order_header_.mcl_b,
                          orm_info.header.size_a,
                          orm_info.header.size_b);

                    int i = 0;
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
            order_header_.magic = headers::ORDER_645_2_MAGIC;
            order_header_.sorm_id = sorm.sorm_id;
            order_header_.object_id = sorm.object_id;
            order_header_.call_id = sorm.call_id;
            order_header_.mcl_a = sorm.mcl_a;
            order_header_.mcl_b = sorm.mcl_b;
            order_header_.conn_param = sorm.conn_param;

            DLOGT(LOG_BEGIN("setSorm(&%x): {%d, %d, %d, %d, %d, %d, %d, %d, %d}"), DLOG_POINTER(&sorm),
                  order_header_.sorm_id,
                  order_header_.object_id,
                  order_header_.call_id,
                  order_header_.mcl_a,
                  order_header_.mcl_b,
                  order_header_.conn_param
                  );

        }
    }
}
