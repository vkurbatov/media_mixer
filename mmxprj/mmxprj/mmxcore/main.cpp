#include <iostream>

#include "mmxlib/logs/log.h"
#include "mmxlib/ipc/pchannel.h"
#include "mmxlib/headers/media.h"
#include "mmxlib/headers/datapack.h"
#include "fcntl.h"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

static char buff[1600];


int main(int argc, char* argv[])
{
    mmx::logs::log_init("/tmp/mmx_core", DEFAULT_LEVEL_LOG, false);

    mmx::ipc::PipeChannel channelIn;

    mmx::ipc::PipeChannel channelOut;

    channelIn.Open("/tmp/pmmxlst-1", O_RDWR, 0777);
    channelOut.Open("/tmp/pmmxsrv-1", O_RDWR, 0777);

    int rc = 0;

    mmx::headers::DATA_PACK &dpack = *(mmx::headers::PDATA_PACK)buff;

    while ((rc = channelIn.Read(buff, sizeof(buff))) >= 0)
    {
        //mmx::logs::logD("Recieved %d bytes from pipe %s", rc, channel.Name());

        std::cout << "Recieved " << rc << " bytes from pipe. Packet id = " << dpack.header.pack_id << ", length = " << dpack.header.length << std::endl;

        //std::cout <<

        rc = channelOut.Write(buff, rc);
    }

    channelIn.Close();
    channelOut.Close();

    mmx::logs::log_init();

    return 0;
}

