#include <iostream>

#include "mmxlib/logs/log.h"
#include "mmxlib/ipc/pchannel.h"
#include "mmxlib/headers/media.h"
#include "mmxlib/headers/datapack.h"
#include "mmxlib/names.h"

#include "fcntl.h"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

static char buff[1600 * 10];


int main(int argc, char* argv[])
{
    mmx::logs::log_init("/tmp/mmx_core", DEFAULT_LEVEL_LOG, false);

    mmx::ipc::PipeChannel channelIn;

    mmx::ipc::PipeChannel channelOut;

    char in_channel_name[256];
    char out_channel_name[256];

    std::sprintf(in_channel_name, MMX_LISTENER_CHANNEL_PATTERN, 2);
    std::sprintf(out_channel_name, MMX_SERVER_CHANNEL_PATTERN, 1);

    int rc = channelIn.Open(in_channel_name, O_RDWR, 0777);
    rc = channelOut.Open(out_channel_name, O_RDWR, 0777);


    while ((rc = channelIn.Read(buff, sizeof(buff))) >= 0)
    {
        //mmx::logs::logD("Recieved %d bytes from pipe %s", rc, channel.Name());

        mmx::headers::DATA_PACK &dpack = *(mmx::headers::PDATA_PACK)buff;

        std::cout << "Recieved " << rc <<
                     " bytes from pipe. Packet id = " << dpack.header.pack_id <<
                     ", length = " << dpack.header.length <<
                     ", blocks = " << dpack.header.block_count <<
                     std::endl;

        //std::cout <<

        rc = channelOut.Write(buff, rc);
    }

    channelIn.Close();
    channelOut.Close();

    mmx::logs::log_init();

    return 0;
}

