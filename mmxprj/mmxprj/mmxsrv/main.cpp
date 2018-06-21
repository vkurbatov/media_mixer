#include <iostream>

#include <cstring>

#include "mmxlib/logs/log.h"
#include "server.h"

#include <netdb.h>      // INADDR_ANY
#include <signal.h>

#include "defines.h"
//#include "../build_number.h"



int main(int argc, char* argv[])
{

    mmxsrv::SERVER_CONFIG config;

    std::memset(&config, 0, sizeof(config));

    config.channel = 1;
    config.interval = DEFAULT_INTERVAL;
    config.address = INADDR_ANY;
    config.port = DEFAULT_BASE_PORT;
    config.mode = mmxsrv::ORM_LINK_TCP;

    mmx::logs::log_level_t  log_level = DEFAULT_LEVEL_LOG;

    int rc = parse_args(argc, argv, &config, nullptr, log_level);

    if (rc == 0)
    {

        char log_file[256];

        std::sprintf(log_file, LOG_NAME_PATTERN, config.channel);

        mmx::logs::log_init(log_file, log_level, false);

        log_file[0] = '\0';
        for (int i = 1; i < argc; i++)
        {
            if (i > 0)
            {
                std::strcat(log_file, " ");
            }
            std::strcat(log_file, argv[i]);
        }

        mmx::logs::logI("@%s Ver=%d.%d.%d.%s (%s) Started!\nArguments: %s\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_STATUS, BUILD_TIME, log_file);

        mmxsrv::Server server(config);

        sig_set();

        rc = server.Execute();

        mmx::logs::logI("@%s Ver=%d.%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }

    return 0;
}
