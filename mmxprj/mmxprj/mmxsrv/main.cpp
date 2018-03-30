#include <iostream>

#include "defines.h"
#include "mmxlib/logs/log.h"
#include "mmxlib/net/socket.h"

#define DEFAULT_LEVEL_LOG mmx::logs::L_ERROR

static char buff[1600];


int main(int argc, char* argv[])
{
    int rc = -1; 

    mmx::net::Socket s(1);

    //rc = s.Open(mmx::net::Socket::StoA("1.0.0.127"), 5000);

    mmx::logs::log_init("/tmp/mmx_srv", DEFAULT_LEVEL_LOG, false);

    rc = mmxsrv::server("/tmp/mmx0002", mmx::net::Socket::StoA("127.0.0.1"), 6000);

    mmx::logs::log_init();

    return 0;
}

