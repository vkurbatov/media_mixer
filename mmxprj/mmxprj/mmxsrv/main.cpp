#include <iostream>

#include "defines.h"

#include "mmxlib/logs/log.h"
#include "server.h"

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Udp media stream listener"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_STATUS "debug"

#define LOG_NAME_PATTERN "/var/log/mmx/server-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_ERROR


int main(int argc, char* argv[])
{
    int rc = -1; 

    mmx::logs::log_level_t log_level = DEFAULT_LEVEL_LOG;

    unsigned char channel = 1;

    auto tcp_address = mmx::net::Socket::StoA("127.0.0.1");
    auto udp_address = mmx::net::Socket::StoA("127.0.0.1");
    mmx::net::port_t tcp_port = 5500;
    mmx::net::port_t udp_port = 5600;

    char log_file[256];

    std::sprintf(log_file, LOG_NAME_PATTERN, channel);

    mmx::logs::log_init(log_file, DEFAULT_LEVEL_LOG, false);


    mmxsrv::Server server(channel, tcp_address, tcp_port, udp_address, udp_port);

    server.Execute();

    // rc = mmxsrv::server("/tmp/mmx0002", mmx::net::Socket::StoA("127.0.0.1"), 6000);

    mmx::logs::log_init();

    return 0;
}

