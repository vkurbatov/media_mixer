#ifndef _MMXSRV_SERVER_H
#define _MMXSRV_SERVER_H

#include <list>
#include <vector>

#include "mmxlib/tools/orm_server.h"
#include "mmxlib/tools/sangoma_media_client.h"
#include "mmxlib/tools/pipe_input_channel.h"

#include "mmxlib/sniffers/dpsniffer.h"
#include "mmxlib/headers/pultstat.h"

namespace mmxsrv
{

    typedef struct _SERVER_CONFIG
    {
        unsigned char                       channel;
        mmx::net::address_t                 address;
        mmx::net::port_t                    port;
        bool                                pult;
        int                                 interval;
    }SERVER_CONFIG, *PSERVER_CONFIG;

    class Server
    {
        mmx::net::SelectExtension           select_;
        mmx::tools::SangomaMediaClient      sangoma_;
        mmx::tools::OrmServer               orm_server_;
        mmx::tools::PipeInputChannel        input_channel_;

        mmx::sniffers::DataPackSniffer      dp_sniffer_;

        mmx::headers::PULT_STAT             stat_;
        mmx::net::timeout_t                 timeout_;
        mmx::tools::Timer                   timer_;

        SERVER_CONFIG                       config_;

        union
        {
            mmx::headers::ORM_INFO_PACKET   orm_info_;
            char orm_raw_[sizeof(mmx::headers::ORDER_645_2_HEADER) + mmx::headers::ORDER_645_2_MAX_DATA_SIZE + 1];
        };




    public:

        Server(const SERVER_CONFIG& config);

        int Execute();

    private:

        // методы конвеера
        void init();
        int getTimeouts();
        void dispatchAll(mmx::tools::dispatch_flags_t dispatch);
        void processInput();
        void closeAll();
        void updateStatistic(void* data);

    };
}

#endif
