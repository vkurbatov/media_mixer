#include <iostream>

#include "mmxlib/logs/log.h"
#include "mmxlib/ipc/pchannel.h"
#include "mmxlib/headers/media.h"
#include "mmxlib/headers/datapack.h"
#include "mmxlib/names.h"
#include "mmxlib/net/select_ex.h"

#include "fcntl.h"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

static char buff[1600 * 10];


int main(int argc, char* argv[])
{
    mmx::logs::log_init("/tmp/mmx_core", DEFAULT_LEVEL_LOG, false);

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

    mmx::logs::log_init();

    return 0;
}

