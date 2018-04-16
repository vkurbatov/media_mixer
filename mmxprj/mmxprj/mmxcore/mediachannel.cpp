#include "mediachannel.h"

#include <cstring>

namespace mmxmux
{
    MediaChannel::MediaChannel() :
        media_pool_(nullptr),
        stream_a_(nullptr),
        stream_b_(nullptr)
    {
        std::memset(&sorm_, 0, sizeof(sorm_));
        std::memset(&orm_header_, 0, sizeof(orm_header_));

    }

    MediaChannel::MediaChannel(MediaChannel&& channel) :
        media_pool_(channel.media_pool_),
        stream_a_(channel.stream_a_),
        stream_b_(channel.stream_b_),
        sorm_(channel.sorm_),
        orm_header_(channel.orm_header_)
    {

        channel.media_pool_ = nullptr;
        channel.stream_a_ = nullptr;
        channel.stream_b_ = nullptr;
        std::memset(&sorm_, 0, sizeof(sorm_));
        std::memset(&orm_header_, 0, sizeof(orm_header_));

    }

    MediaChannel& MediaChannel::operator=(MediaChannel&& channel)
    {
        media_pool_ = channel.media_pool_;
        stream_a_ = channel.stream_a_;
        stream_b_ = channel.stream_b_;
        sorm_ = channel.sorm_;
        orm_header_ = channel.orm_header_;

        channel.media_pool_ = nullptr;
        channel.stream_a_ = nullptr;
        channel.stream_b_ = nullptr;
        std::memset(&sorm_, 0, sizeof(sorm_));
        std::memset(&orm_header_, 0, sizeof(orm_header_));
    }

    MediaChannel::~MediaChannel()
    {

        Drop();
        SetProxy();

    }

    int MediaChannel::SetProxy(const mmx::headers::SANGOMA_PROXY_INFO* proxy)
    {

        int rc = 0;

        if (media_pool_ != nullptr)
        {

            // сперва освободим имеющиеся медиапотоки

            if (stream_a_ != nullptr)
            {
                media_pool_->Release(stream_a_);
                stream_a_ = nullptr;
            }

            if (stream_b_ != nullptr)
            {
                media_pool_->Release(stream_b_);
                stream_b_ = nullptr;
            }

            // если проксирование не задано, то вызов был для освобождения ресурсов

            if (proxy != nullptr)
            {
                stream_a_ = media_pool_->GetStream(proxy->source_a.address, proxy->source_a.port);
                stream_b_ = media_pool_->GetStream(proxy->source_a.address, proxy->source_b.port);

                rc = (int)(stream_a_ != nullptr) + (stream_b_ != nullptr);
            }
        }


        return 0;
    }

    const mmx::headers::SANGOMA_SORM_INFO& MediaChannel::GetOrmInfo() const
    {
        return sorm_;
    }


    int MediaChannel::OrmInfoPack(void* data, int size)
    {
        int rc = 0;

        const mmx::headers::MEDIA_SAMPLE* sample_a = nullptr;
        const mmx::headers::MEDIA_SAMPLE* sample_b = nullptr;

        unsigned short size_a = 0, size_b = 0;

        if (stream_a_ != nullptr && (sample_a = stream_a_->GetSample()) != nullptr)
        {
            size_a = sample_a->header.length;
        }

        if (stream_b_ != nullptr && (sample_b = stream_b_->GetSample()) != nullptr)
        {
            size_b = sample_b->header.length;
        }

        // если не задан буффер, то просто возвращаем количество необходимых байт

        rc = size_a + size_b + (int)sizeof(mmx::headers::ORM_INFO_HEADER);

        if (data != nullptr)
        {
            if (size >= rc)
            {

                mmx::headers::ORM_INFO_PACKET& orm_info = *(mmx::headers::ORM_INFO_PACKET*)data;
                orm_info.header.size_a = size_a;
                orm_info.header.size_b = size_b;

                orm_header_.order_header.block_number++;
                orm_header_.order_header.packet_id++;               
                orm_header_.order_header.conn_flag = (int)(size_a == 0 && size_b == 0);

                orm_info.header = orm_header_;

                auto size_max = size_a > size_b ? size_a : size_b;

                int i = 0;

                // КСЛ A

                for (i = 0;i < size_a; i++)
                {
                    orm_info.data[i << 1] = sample_a->media[i];
                }
                while (i < size_max)
                {
                    orm_info.data[i++ << 1] = 0x7F;
                }

                // КСЛ B

                for (i = 0;i < size_b; i++)
                {
                    orm_info.data[i << 1 + 1] = sample_b->media[i];
                }
                while (i < size_max)
                {
                    orm_info.data[i++ << 1 + 1] = 0x7F;
                }

            }
        }

        return rc;
    }

    void MediaChannel::Drop()
    {

        // пока не знаю что сюда написать
    }

    void MediaChannel::setSorm(const mmx::headers::SANGOMA_SORM_INFO& sorm)
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
