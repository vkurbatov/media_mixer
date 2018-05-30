#include "mux.h"
#include "mmxlib/names.h"

#include "mmxlib/ipc/sem.h"

#include "mmxlib/headers/pultstat.h"

#include <cstring>
#include <cstdio>

#define MUX_UNIQUE_BASE_KEY
#define SRV_SHMEM_UNIQUE_BASE_KEY   1200

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("Mux", msg)

namespace mmxmux
{

    Mux::Mux(const MUX_CONFIG& config) :
        config_(config),
        sangoma_(config.sgm_address, config.sgm_port, select_, config.interval),
        input_channel_(MMX_LISTENER_CHANNEL_PATTERN, config.channel_num, select_),
        output_channel_pool_(MMX_SERVER_CHANNEL_PATTERN),
        sorm_pool_(media_pool_, 10, config.mixed_gain)
    {
        timer_.Start(config_.media_period);
        std::memset(channel_indexes_, -1, sizeof(channel_indexes_));
    }

    void Mux::test()
    {
        mmx::headers::SANGOMA_PROXY_INFO proxy;
        mmx::headers::SANGOMA_SORM_INFO sorm;


        proxy.source_a.address = config_.sgm_address;
        proxy.source_a.port = 5062;

        proxy.source_b = proxy.source_a;
        proxy.source_b.port = 5063;

        sorm.call_id = 1;
        sorm.channel_id = 1;
        sorm.conn_param = 1;
        sorm.mcl_a = 5;
        sorm.mcl_b = 5;
        sorm.object_id = 8;
        sorm.sorm_id = 9;

        sorm_pool_.GetSorm(sorm, proxy);
    }

    int Mux::Execute()
    {

        int rc = 0;

        mmx::ipc::Semaphore sem(MUX_UNIQUE_BASE_KEY + config_.channel_num, 0666);

        if (sem.Open() < 0 || sem.Get() > 0)
        {
            DLOGE(LOG_BEGIN("Execute(): duplicate instance, channel = %d!!"), config_.channel_num);
            return -EBUSY;
        }

        sem.Set();

        init();

        // test();


        while(1)
        {

            rc = select_.Wait(getTimeouts());

            if (rc >= 0 || rc == -ETIMEDOUT)
            {
                if (rc == -ETIMEDOUT)
                {
                    dispatchAll(mmx::tools::DISPATCH_TIMER);
                    timerWork();
                }
                else
                {

                    dispatchAll(mmx::tools::DISPATCH_IO);
                    processInput();
                    processSangoma();
                }

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

        int ch_num = 0;

        for (int i = 1; i < 256; i++)
        {
            if ((config_.channels[i / 8] & (1 << (i % 8))) != 0)
            {
                output_channel_pool_.GetChannel(i, select_, config_.interval);
                shmem_servers_.push_back(std::move(mmx::ipc::SharedMemory(SRV_SHMEM_UNIQUE_BASE_KEY + i)));
                channel_indexes_[i] = ch_num++;
            }
            else
            {
                channel_indexes_[i] = -1;
            }

        }

        dispatchAll(mmx::tools::DISPATCH_INIT);
    }

    void Mux::dispatchAll(mmx::tools::dispatch_flags_t dispatch)
    {

        input_channel_.Dispatch(dispatch);

        for (auto& o : output_channel_pool_.GetChannels())
        {
            bool fconn = o.IsDown();

            o.Dispatch(dispatch);

            if (fconn == true && !o.IsDown())
            {

                int idx = channel_indexes_[o.GetChannelId()];
                shmem_servers_[idx].Close();
                shmem_servers_[idx].Open();

            }
        }

        sangoma_.Dispatch(dispatch);

        for (auto& c : sangoma_.GetClients())
        {
            c.Dispatch(dispatch);
        }

        sangoma_.GetClients().remove_if([](mmx::tools::SangomaClient& c) { return c.IsDown(); });
    }

    int Mux::getTimeouts()
    {
        int rc = timer_.Left();

        int to = input_channel_.QueryOrderTimeout();

        if (to >= 0 && (to < rc || rc < 0))
        {
            rc = to;
        }

        for (auto& o : output_channel_pool_.GetChannels())
        {
            to = o.QueryOrderTimeout();
            if (to >= 0 && (to < rc || rc < 0))
            {
                rc = to;
            }
        }

        to = sangoma_.QueryOrderTimeout();

        if (to >= 0 && (to < rc || rc < 0))
        {
            rc = to;
        }

        return rc;

    }

    void Mux::timerWork()
    {
        if (timer_.IsEnable())        
        {
            if (!rm_sorms_.empty())
            {
                for (auto& m : rm_sorms_)
                {
                    auto channel = output_channel_pool_[m->GetOrmInfo().channel_id];

                    if (channel != nullptr)
                    {
                        m->OrmInfoPack(channel->GetWritter(), 1);
                    }

                    sorm_pool_.Release(m);
                }

                rm_sorms_.clear();
            }


            for (auto& m : sorm_pool_.GetSorms())
            {
                auto channel = output_channel_pool_[m->GetOrmInfo().channel_id];

                if (channel != nullptr)
                {
                    m->OrmInfoPack(channel->GetWritter());

                }                              

            }                     

            for (auto& c : output_channel_pool_.GetChannels())
            {
                c.Send();
            }

            timer_.Start(config_.media_period);
        }
    }

    void Mux::processInput()
    {
        const char* data = (const char*)input_channel_.Data();
        int size = input_channel_.Size();

        if (data != nullptr && size > 0)
        {

            int ret = 1;

            int off = 0;

            while (ret > 0 && off < size)
            {

                ret = dp_sniffer_.PutStream(data + off, size - off);


                if (dp_sniffer_.IsComplete())
                {

                    auto dp = dp_sniffer_.GetDataPacket();

                    if (dp != nullptr)
                    {

                        mmx::data::DataPacketReader reader(&dp->header);


                        auto block = reader.GetBlock();

                        while(block != nullptr)
                        {
                            DLOGD(LOG_BEGIN("processInput(): recieved %d bytes from %d block, pack_id = %d"), block->header.length - sizeof(block->header), block->header.block_id, dp->header.pack_id);

                            // mmx::logs::logI("@Recieved %d bytes from [%d:%d]", block->header.length - sizeof(block->header), dp->header.pack_id, block->header.block_id);

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

                off += ret > 0 ? ret : 0;

            }

        input_channel_.Drop();

        }
    }

    int get_query_info(const mmx::headers::SANGOMA_PACKET& sp, char *out_buff, int sorms)
    {
        char ip_a[16], ip_b[16];
        int rc = std::sprintf(out_buff,"proxy: \n\tsource_a = %s:%d;\n\tsource_b = %s:%d;\nsorm_count = %d;\n",
                     mmx::net::Socket::AtoS(sp.q_proxy.proxy.source_a.address, ip_a),
                     sp.q_proxy.proxy.source_a.port,
                     mmx::net::Socket::AtoS(sp.q_proxy.proxy.source_b.address, ip_b),
                     sp.q_proxy.proxy.source_b.port,
                     sorms);
        for (int i = 0; i < sorms; i++)
        {
            rc += std::sprintf(out_buff + rc,
                               "Sorm[%d]\n\tcall_id = %d\n\tchannel_id = %d\n\tconn_param = %d\n\tmcl_a = %d\n\tmcl_b = %d\n\tobject_id=%d\n\tsorm_id = %d\n",
                               i,
                               (int)sp.q_proxy.sorm[i].call_id,
                               (int)sp.q_proxy.sorm[i].channel_id,
                               (int)sp.q_proxy.sorm[i].conn_param,
                               (int)sp.q_proxy.sorm[i].mcl_a,
                               (int)sp.q_proxy.sorm[i].mcl_b,
                               (int)sp.q_proxy.sorm[i].object_id,
                               (int)sp.q_proxy.sorm[i].sorm_id);
        }

        return rc;
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

            mmx::headers::PULT_STAT* plt_stat = (mmx::headers::PULT_STAT*)shmem_servers_[channel_indexes_[answer.a_link_status[i].num]].Data();

            answer.a_link_status[i].status = !o.IsDown() && plt_stat != nullptr && plt_stat->online_conn > 0;

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
                        DLOGD(LOG_BEGIN("processSangoma(): recieve PASSWORD_STATUS query, len = %d"), query->header.length);

                        // LogIN

                        for (auto& s : sorm_pool_.GetSorms())
                        {
                            rm_sorms_.push_back(s);
                        }
                        break;
                    case mmx::headers::SI_LINK_STATUS:
                        DLOGT(LOG_BEGIN("processSangoma(): recieve LINK_STATUS query, len = %d"), query->header.length);
                        break;
                    case mmx::headers::SI_START_PROXY:
                    case mmx::headers::SI_END_PROXY:
                        {
                            int sorms = (query->header.length - sizeof(query->q_proxy.proxy)) / sizeof(query->q_proxy.sorm[0]);

                            static char sbuf[1024];

                            get_query_info(*query, sbuf, sorms);


                            for (int i = 0; i < sorms; i++)
                            {
                                if (query->header.type == mmx::headers::SI_START_PROXY)
                                {

                                    DLOGI(LOG_BEGIN("processSangoma(): recieve START_PROXY query (%d), len = %d;\nquery_info:\n%s"), query->header.type, query->header.length, sbuf);

                                    auto s = sorm_pool_.GetSorm(query->q_proxy.sorm[i], query->q_proxy.proxy);
                                    if (s != nullptr)
                                    {
                                        s->Reset();
                                    }

                                }
                                else
                                {                                    
                                    DLOGI(LOG_BEGIN("processSangoma(): recieve STOP_PROXY query (%d), len = %d;\nquery_info:\n%s"), query->header.type, query->header.length, sbuf);

                                    auto m = sorm_pool_.FindSorm(query->q_proxy.sorm[i]);

                                    if (m != nullptr)
                                    {
                                        rm_sorms_.push_back(m);

                                        auto& diag = m->GetDiagInfo();

                                        DLOGI(LOG_BEGIN("processSangoma(): proxy status: rtp1 = %d:%d, rtp2 = %d:%d, order645 = %d:%d"),
                                              diag.rtp_packs[0], diag.rtp_bytes[0], diag.rtp_packs[1], diag.rtp_bytes[1], diag.order645_packs, diag.order645_bytes);
                                    }
                                }
                            }

                        }
                        break;
                    default:
                        DLOGW(LOG_BEGIN("processSangoma(): recieve unknown query %d, len = %d"), query->header.type, query->header.length);
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

        for (auto& s : shmem_servers_)
        {
            s.Close();
        }

        sangoma_.Close();

        sangoma_.GetClients().clear();

    }

}
