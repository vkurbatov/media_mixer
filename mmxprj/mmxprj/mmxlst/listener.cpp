#include "listener.h"

#include <cstring>

#include <netdb.h>  // SOCK_RAW, IPPROTO_UDP
#include <fcntl.h>  // O_NONBLOCK
#include <errno.h>
#include <sys/time.h>   // gettimeofday
#include <errno.h>

#include "mmxlib/logs/log.h"
#include "mmxlib/headers/udp.h"
#include "mmxlib/headers/media.h"
#include "mmxlib/names.h"


#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("Listener", msg)

#define MAX_DEFFERED_SIZE   100
#define DEFAULT_TIMEOUT     2000



namespace mmxlst
{
    Listener::Listener(const LISTENER_CONFIG& config, const mmx::net::PortSet& ports) :
        config_(config),
        output_channel_(MMX_LISTENER_CHANNEL_PATTERN, config.channel, select_, config.interval),
        udp_listener_(ports, config.address, output_channel_.GetWritter(), packet_pool_, select_, config.interval)
    {

    }

    Listener::~Listener()
    {

    }

    int Listener::Execute()
    {

        int rc = 0;

        init();

        while(1)
        {

            rc = select_.Wait(getTimeouts());

            if (rc > 0 || rc == -ETIMEDOUT)
            {
                dispatchAll(rc == -ETIMEDOUT ? mmx::tools::DISPATCH_TIMER : mmx::tools::DISPATCH_IO);
                processData();
            }
            else
            {
                closeAll();
            }
        }
    }

    void Listener::init()
    {
        dispatchAll(mmx::tools::DISPATCH_INIT);
    }

    int Listener::getTimeouts()
    {
        int rc = mmx::net::INFINITE_TIMEOUT;

        mmx::tools::IChannelDispatcher* channels[] =
        {
            &udp_listener_,
            &output_channel_

        };

        for (int i = 0 ; i < 2; i++)
        {
            int to = channels[i]->QueryOrderTimeout();

            if (to >= 0 && (to < rc || rc < 0))
            {
                rc = to;
            }
        }

        return rc;

    }

    void Listener::dispatchAll(mmx::tools::dispatch_flags_t dispatch)
    {

        udp_listener_.Dispatch(dispatch);
        output_channel_.Dispatch(dispatch);

    }

    void Listener::closeAll()
    {

        udp_listener_.Close();
        output_channel_.Close();

        select_.Reset();

    }

    void Listener::processData()
    {
        if (output_channel_.IsReadyData())
        {
            output_channel_.Send();
        }
    }
}
