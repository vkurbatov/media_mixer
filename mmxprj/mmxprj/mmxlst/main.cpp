
/* Сервис-слушатель UDP портов. Читает
 *
 *
 *
 *
 */

#include <iostream>
#include <cstring>

#include <signal.h> // sigpipe
#include <netdb.h>  // INADDR_ANY

#include "listener.h"

#include "defines.h"

//#include "../build_number.h"

mmx::logs::log_level_t  g_log_level = mmx::logs::L_DEBUG;

static int parse_ports(char* params, mmx::net::PortSet& ports);
static int parse_args(int argc, char* argv[], mmxlst::LISTENER_CONFIG& config, mmx::net::PortSet& ports, mmx::logs::log_level_t& log_level);

int main(int argc, char* argv[])
{

    mmxlst::LISTENER_CONFIG lst_config;

    mmx::net::PortSet ports;

    lst_config.address = 0;
    lst_config.channel = 1;
    lst_config.interval = 2000;

    int rc = parse_args(argc, argv, &lst_config, &ports, g_log_level);

    if (rc == 0)
    {

        char log_file[256];

        std::sprintf(log_file, LOG_NAME_PATTERN, lst_config.channel);

        mmx::logs::log_init(log_file, g_log_level, false);

        log_file[0] = '\0';

        for (int i = 1; i < argc; i++)
        {
            if (i > 0)
            {
                std::strcat(log_file, " ");
            }
            std::strcat(log_file, argv[i]);
        }

        mmx::logs::logI("@%s Ver=%d.%d.%d.%s (%d) Started!\nArguments: %s\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_STATUS, BUILD_TIME, log_file);

        mmxlst::Listener listener(lst_config, ports);

        sig_set();

        rc = listener.Execute();

        mmx::logs::logI("@%s Ver=%d.%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }

    return rc;
}

