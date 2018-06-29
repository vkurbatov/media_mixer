#include <iostream>

#include <cstring>
#include <signal.h> // sigpipe

#include "mmxlib/logs/log.h"


#include "mux.h"

#include <fcntl.h>
#include <netdb.h>  //INADDR_NONE

#include "defines.h"

int main(int argc, char* argv[])
{
    mmxmux::MUX_CONFIG config;
    mmx::logs::log_level_t log_level = DEFAULT_LEVEL_LOG;

    std::memset(&config, 0, sizeof(config));

    config.channel_num = 1;
    config.interval = DEFAULT_INTERVAL;
    config.media_period = DEFAULT_MEDIA_PERIOD;
    config.sgm_address = INADDR_ANY;
    config.sgm_port = DEFAULT_BASE_PORT;
    config.mixed_gain = DEFAULT_MIXED_GAIN;
    config.jitter_size = DEFAULT_JITTER_BUFFER_SIZE;
    config.mute_time = DEFAULT_MUTE_TIME;
    config.media_health_time = DEFAULT_MEDIA_HEALTH_TIME;

    int rc = parse_args(argc, argv, &config, nullptr, log_level);

    if (rc == 0)
    {

        char log_file[256];

        std::sprintf(log_file, LOG_NAME_PATTERN, config.channel_num);

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

        sig_set();

        mmxmux::Mux Mux(config);

        rc = Mux.Execute();

        mmx::logs::logI("@%s Ver=%d.%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }

    return 0;
}


