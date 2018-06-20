#ifndef _MMXMUX_MUX_H
#define _MMXMUX_MUX_H


#include "mmxlib/net/select_ex.h"

#include "mmxlib/tools/timer.h"
#include "mmxlib/tools/pipe_input_channel.h"
#include "mmxlib/tools/pipe_output_channel_pool.h"
#include "mmxlib/tools/sangoma_server.h"

#include "mmxlib/data/datapacket.h"

#include "mmxlib/headers/order645_2.h"
#include "mmxlib/headers/si.h"

#include "mmxlib/sniffers/dpsniffer.h"

#include "mmxlib/media/mediapool.h"
#include "mmxlib/media/sormpool.h"
#include "mmxlib/ipc/shmem.h"


namespace mmxmux
{
    typedef struct _MUX_CONFIG
    {

        mmx::net::address_t                 sgm_address;
        mmx::net::port_t                    sgm_port;
        int                                 interval;
        int                                 media_period;
        char                                channel_num;
        unsigned char                       mixed_gain;
        unsigned int                        jitter_size;
        unsigned int                        mute_time;
        unsigned char                       channels[32];

    }MUX_CONFIG,*PMUX_CONFIG;

    class Mux
    {

        MUX_CONFIG                          config_;

        mmx::net::SelectExtension           select_;

        mmx::media::MediaPool               media_pool_;
        mmx::media::SormPool                sorm_pool_;

        mmx::tools::SangomaServer           sangoma_;
        mmx::tools::Timer                   timer_;

        mmx::tools::PipeInputChannel        input_channel_;
        mmx::tools::PipeOutputChannelPool   output_channel_pool_;


        mmx::sniffers::DataPackSniffer      dp_sniffer_;

        std::vector<mmx::ipc::SharedMemory> shmem_servers_;
        std::vector<mmx::media::Sorm*>      rm_sorms_;
        int                                 channel_indexes_[256];



    public:

        Mux(const MUX_CONFIG& config);

        int Execute();

    private:

        void init();
        int getTimeouts();
        void dispatchAll(mmx::tools::dispatch_flags_t dispatch);
        void processInput();
        void timerWork();
        void processSangoma();
        void closeAll();
        void test();

    };
}

#endif
