#include "mux.h"
#include "mmxlib/names.h"

#include <cstring>

#define MEDIA_PERIOD


namespace mmxmux
{

    Mux::Mux(const MUX_CONFIG& config) :
        config_(config),
        sangoma_(config.sgm_address, config.sgm_port, select_, config.interval),
        input_channel_(MMX_LISTENER_CHANNEL_PATTERN, config.channel_num, select_),
        output_channel_pool_(MMX_SERVER_CHANNEL_PATTERN),
        media_channel_pool_(media_pool_, 10)
    {
        timer_.Start(config_.media_period);
    }

    void Mux::test()
    {

    }

    int Mux::Execute()
    {

        int rc = 0;

        init();

        while(1)
        {

            rc = select_.Wait(getTimeouts());

            if (rc >= 0 || rc == -ETIMEDOUT)
            {
                dispatchAll();
                processInput();
                timerWork();
                processSangoma();

            }
            else
            {
                closeAll();
            }
        }


        return rc;

    }

    void Mux::init()
    {
        for (int i = 1; i < 256; i++)
        {
            if ((config_.channels[i / 8] & (1 << (i % 8))) != 0)
            {
                output_channel_pool_.GetChannel(i, select_, config_.interval);
            }
        }
    }

    void Mux::dispatchAll()
    {
        input_channel_.Dispatch();
        for (auto& o : output_channel_pool_.GetChannels())
        {
            o.Dispatch();
        }

        sangoma_.Dispatch();

        for (auto& c : sangoma_.GetClients())
        {
            c.Dispatch();
        }

        sangoma_.GetClients().remove_if([](mmx::tools::SangomaClient& c) { return c.IsDown(); });
    }

    int Mux::getTimeouts()
    {
        int rc = timer_.Left();

        int to = input_channel_.GetLeftTimeWork();

        if (to >= 0 && to < rc)
        {
            rc = to;
        }

        for (auto& o : output_channel_pool_.GetChannels())
        {
            to = o.GetLeftTimeWork();
            if (to >= 0 && to < rc)
            {
                rc = to;
            }
        }

        to = sangoma_.GetLeftTimeWork();

        if (to >= 0 && to < rc)
        {
            rc = to;
        }

        return rc;

    }

    void Mux::timerWork()
    {
        if (timer_.IsEnable())
        {
            for (auto& m : media_channel_pool_.GetChannels())
            {
                auto channel = output_channel_pool_.GetChannel(m->GetOrmInfo().channel_id, select_, config_.interval);

                if (channel != nullptr)
                {
                    int size = m->OrmInfoPack(nullptr, 0);

                    if (size > 0)
                    {
                        auto block = channel->GetWritter().QueryBlock(size);

                        if (block != nullptr)
                        {
                            size = m->OrmInfoPack(block->data, size);

                            if (size > 0)
                            {
                                channel->GetWritter().Commit();
                            }

                        }

                    }
                }

            }

            for (auto& c : output_channel_pool_.GetChannels())
            {
                c.Send();
                c.GetWritter();
            }

            timer_.Start(config_.media_period);
        }
    }

    void Mux::processInput()
    {
        const void* data = input_channel_.Data();
        int size = input_channel_.Size();

        if (data != nullptr && size > 0)
        {

            int ret = size;

            while (ret > 0)
            {

                ret = dp_sniffer_.PutStream(data, ret);


                if (dp_sniffer_.IsComplete())
                {

                    auto dp = dp_sniffer_.GetDataPacket();

                    if (dp != nullptr)
                    {

                        mmx::data::DataPacketReader reader(&dp->header);


                        auto block = reader.GetBlock();

                        while(block != nullptr)
                        {

                            mmx::headers::MEDIA_DATA& media = *(mmx::headers::MEDIA_DATA*)block->data;


                            auto stream = media_pool_.FindStream(media.header.net_points.destination.address, media.header.net_points.destination.port);

                            if (stream != nullptr)
                            {
                                stream->PutSample(media);
                            }

                            block = reader.GetBlock();
                        }
                    }

                    dp_sniffer_.Drop();

                }

                if (ret > 0)
                {
                    ret = size - ret;
                }

            }

        }
    }

    void Mux::processSangoma()
    {
        mmx::headers::SANGOMA_PACKET answer;

        std::memset(&answer, 0, sizeof(answer));

        answer.header.type = mmx::headers::SI_LINK_STATUS;
        answer.header.length = 0;

        int i = 0;

        for (mmx::tools::PipeOutputChannel& o : output_channel_pool_.GetChannels())
        {
            answer.a_link_status[i].type = mmx::headers::SI_LINK_TCP;
            answer.a_link_status[i].num = o.GetChannelId();
            answer.a_link_status[i].status = 1;
            i++;
        }

        answer.header.length += i * sizeof(answer.a_link_status[0]);

        for (mmx::tools::SangomaClient& c : sangoma_.GetClients())
        {
            auto query = c.GetQuery();

            if (query != nullptr)
            {
                switch (query->header.type)
                {
                    case mmx::headers::SI_PASSWORD_STATUS:
                    case mmx::headers::SI_LINK_STATUS:

                        break;
                    case mmx::headers::SI_START_PROXY:
                    case mmx::headers::SI_END_PROXY:
                        {
                            int sorms = query->header.length - sizeof(query->header) / sizeof(query->a_link_status[0]);

                            for (int i = 0; i < sorms; i++)
                            {
                                if (query->header.type == mmx::headers::SI_START_PROXY)
                                {
                                    media_channel_pool_.GetChannel(query->q_proxy.sorm[i], query->q_proxy.proxy);
                                }
                                else
                                {
                                    media_channel_pool_.Release(query->q_proxy.sorm[i]);
                                }
                            }
                        }
                        break;
                    default:
                        query = nullptr;

                }
                if (query != nullptr)
                {
                    c.PutAnswer(answer);

                }
                c.Drop();

            }
        }
    }

    void Mux::closeAll()
    {

        input_channel_.Close();

        for (auto& o : output_channel_pool_.GetChannels())
        {
            o.Close();
        }

        sangoma_.Close();

        sangoma_.GetClients().clear();

    }

}
