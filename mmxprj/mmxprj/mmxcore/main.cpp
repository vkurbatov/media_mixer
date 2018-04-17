#include <iostream>

#include <cstring>

#include "mmxlib/logs/log.h"

#include "mux.h"

#include "fcntl.h"

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Media stream multiplexor (core)"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_STATUS "debug"

#define LOG_NAME_PATTERN "/var/log/mmx/mediamux-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

static char buff[1600 * 10];


int main(int argc, char* argv[])
{
    mmxmux::MUX_CONFIG config;

    std::memset(&config, 0, sizeof(config));

    config.channel_num = 2;
    config.interval = 2000;
    config.media_period = 160;
    config.sgm_address = mmx::net::Socket::StoA("127.0.0.1");
    config.sgm_port = 5000;
    config.channels[0] |= 1 << 1;


    int rc = 0;

    if (rc == 0)
    {

        char log_file[256];

        std::sprintf(log_file, LOG_NAME_PATTERN, config.channel_num);

        mmx::logs::log_init(log_file, DEFAULT_LEVEL_LOG, false);

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Started!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmxmux::Mux Mux(config);

        rc = Mux.Execute();

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }


    /*
    mmx::ipc::PipeChannel channelIn;

    mmx::ipc::PipeChannel channelOut;

    mmx::net::SelectExtension sel;

    char in_channel_name[256];
    char out_channel_name[256];

    std::sprintf(in_channel_name, MMX_LISTENER_CHANNEL_PATTERN, 2);
    std::sprintf(out_channel_name, MMX_SERVER_CHANNEL_PATTERN, 1);

    int rc = channelIn.Open(in_channel_name, O_RDONLY | O_NONBLOCK, 0777);
    rc = channelOut.Open(out_channel_name, O_WRONLY | O_NONBLOCK);

    sel.Set(channelIn.Handle(), mmx::net::S_EV_READ);

    while ((rc = sel.Wait()) > 0)
    {
        if (sel.IsRead(channelIn.Handle()))
        {
            rc = channelIn.Read(buff, sizeof(buff));

            if (rc > 0)
            {
                mmx::headers::DATA_PACK &dpack = *(mmx::headers::PDATA_PACK)buff;

                std::cout << "Recieved " << rc <<
                             " bytes from pipe. Packet id = " << dpack.header.pack_id <<
                             ", length = " << dpack.header.length <<
                             ", blocks = " << dpack.header.block_count <<
                             std::endl;
                if (channelOut.Handle() >= 0)
                {
                    rc = channelOut.Write(buff, rc);
                }

                // channelIn.Close();
                // break;
            }
        }
    }

    channelIn.Close();
    channelOut.Close();

    */

    mmx::logs::log_init();

    return 0;
}

