#include "sorm.h"
#include "media/codecs/audio/mixer.h"
#include "media/codecs/audio/pcma.h"

#include <cstring>

namespace mmx
{
    namespace media
    {
        Sorm::Sorm() :
            media_pool_(nullptr)
        {
            std::memset(&sorm_, 0, sizeof(sorm_));
            std::memset(&orm_header_, 0, sizeof(orm_header_));
            streams_[0] = nullptr;
            streams_[1] = nullptr;
        }

        Sorm::Sorm(Sorm&& channel) :
            media_pool_(channel.media_pool_),
            sorm_(channel.sorm_),
            orm_header_(channel.orm_header_)
        {
            streams_[0] = channel.streams_[0];
            streams_[1] = channel.streams_[1];
            channel.media_pool_ = nullptr;
            channel.streams_[0] = nullptr;
            channel.streams_[1] = nullptr;
            std::memset(&sorm_, 0, sizeof(sorm_));
            std::memset(&orm_header_, 0, sizeof(orm_header_));

        }

        Sorm& Sorm::operator=(Sorm&& channel)
        {
            media_pool_ = channel.media_pool_;
            streams_[0] = channel.streams_[0];
            streams_[1] = channel.streams_[1];
            sorm_ = channel.sorm_;
            orm_header_ = channel.orm_header_;

            channel.media_pool_ = nullptr;
            channel.streams_[0] = nullptr;
            channel.streams_[1] = nullptr;
            std::memset(&sorm_, 0, sizeof(sorm_));
            std::memset(&orm_header_, 0, sizeof(orm_header_));
        }

        Sorm::~Sorm()
        {

            Drop();
            SetProxy();

        }

        int Sorm::SetProxy(const mmx::headers::SANGOMA_PROXY_INFO* proxy)
        {

            int rc = 0;

            if (media_pool_ != nullptr)
            {

                // сперва освободим имеющиеся медиапотоки

                for (auto& s : streams_)
                {
                    if (s != nullptr)
                    {
                        media_pool_->Release(s);
                        s = nullptr;
                    }
                }

                // если проксирование не задано, то вызов был для освобождения ресурсов

                if (proxy != nullptr)
                {
                    streams_[0] = media_pool_->GetStream(proxy->source_a.address, proxy->source_a.port);
                    streams_[1] = media_pool_->GetStream(proxy->source_b.address, proxy->source_b.port);

                    rc = (int)(streams_[0] != nullptr) + (streams_[1] != nullptr);
                }
            }


            return rc;
        }

        const mmx::headers::SANGOMA_SORM_INFO& Sorm::GetOrmInfo() const
        {
            return sorm_;
        }


        int Sorm::OrmInfoPack(void* data, int size)
        {

            static int err_counter = 0;

            int rc = 0;

            const mmx::headers::MEDIA_SAMPLE* media_samples[2] = { nullptr };

            unsigned short size_arr[2] = { 0 };

            for (int i = 0; i < 2; i++) \

            {

                media_samples[i] = (streams_[i] != nullptr) ?
                            streams_[i]->GetSample() : nullptr;

                if (media_samples[i] != nullptr)
                {
                    rc += size_arr[i] = media_samples[i]->header.length - sizeof(media_samples[i]->header);
                }

            }

            // если не задан буффер, то просто возвращаем количество необходимых байт

            rc += (int)sizeof(mmx::headers::ORM_INFO_HEADER);

            if (data != nullptr)
            {
                if (size >= rc)
                {

                    mmx::headers::ORM_INFO_PACKET& orm_info = *(mmx::headers::ORM_INFO_PACKET*)data;
                    orm_info.header.size_a = size_arr[0];
                    orm_info.header.size_b = size_arr[1];

                    orm_header_.order_header.block_number++;
                    orm_header_.order_header.packet_id++;
                    orm_header_.order_header.conn_flag = (int)(size_arr[0] == 0 && size_arr[1] == 0);

                    orm_info.header = orm_header_;

                    auto size_max = size_arr[0] > size_arr[1] ? size_arr[0] : size_arr[1];

                    if (size_arr[0] != size_arr[1] || size_max == 0)
                    {
                           err_counter++;
                    }

                    if (orm_header_.order_header.mcl_a != orm_header_.order_header.mcl_b)
                    {

                        for (int j = 0; j < 2; j++)
                        {
                            int i = 0;

                            for (i = 0;i < size_arr[j]; i++)
                            {
                                orm_info.data[(i << 1) + j] = media_samples[j]->media[i];
                            }
                            while (i < size_max)
                            {
                                orm_info.data[(i++ << 1) + j] = 0x7F;
                            }
                        }
                    }
                    else
                    {
                        auto size_min = size_arr[0] > size_arr[1] ? size_arr[1] : size_arr[0];

                        int i = 0;
                        for (;i < size_min; i++)
                        {
                            orm_info.data[i] = codecs::audio::PcmaCodec::Encode(
                                codecs::audio::mixer(
                                    codecs::audio::PcmaCodec::Decode(media_samples[0]->media[i]),
                                    codecs::audio::PcmaCodec::Decode(media_samples[1]->media[i]),
                                    70
                                    )
                                );
                        }

                        int j = (int)(size_arr[0] < size_arr[1]);

                        while (i < size_max)
                        {
                            orm_info.data[i++] = media_samples[j]->media[i];
                        }

                        orm_info.header.size_a = size_max;
                        orm_info.header.size_b = 0;
                    }

                }
            }

            return rc;
        }

        void Sorm::Drop()
        {

            // пока не знаю что сюда написать
        }

        void Sorm::setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm)
        {
            if (&sorm != &sorm_)
            {
                sorm_ = sorm;
            }

            orm_header_.order_header.sorm_id = sorm.sorm_id;
            orm_header_.order_header.object_id = sorm.object_id;
            orm_header_.order_header.call_id = sorm.call_id;
            orm_header_.order_header.mcl_a = sorm.mcl_a;
            orm_header_.order_header.mcl_b = sorm.mcl_b;
            orm_header_.order_header.conn_param = sorm.conn_param;
            orm_header_.order_header.block_number = 0;
            orm_header_.order_header.packet_id = 0;
            orm_header_.order_header.magic = mmx::headers::ORDER_645_2_MAGIC;

        }
    }
}
