#include "defines.h"
#include <iostream>

#include <cstring>

#include "mmxlib/net/socket.h"
#include "mmxlib/net/socket.h"

#include <netdb.h>

#define TEST_DATA_SIZE 16

namespace mmxtest
{
    int test5()
    {

        mmx::net::address_t address = mmx::net::Socket::StoA("127.0.0.1");
        mmx::net::port_t port = 6000;
        mmx::net::Socket udp(SOCK_DGRAM, IPPROTO_UDP);



    }
}
