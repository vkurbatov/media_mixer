
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

#include "mmxlib/logs/log.h"
#include "listener.h"

#include "../build_number.h"

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Udp media stream listener"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION BUILD_NUMBER

#ifdef DEBUG
#define SERVICE_STATUS "debug"
#else
#define SERVICE_STATUS "release"
#endif

#define LOG_NAME_PATTERN "/var/log/mmx/listener-%d.log"

mmx::logs::log_level_t  g_log_level = mmx::logs::L_DEBUG;

static int parse_ports(char* params, mmx::net::PortSet& ports);
static int parse_args(int argc, char* argv[], mmxlst::LISTENER_CONFIG& config, mmx::net::PortSet& ports, mmx::logs::log_level_t& log_level);

void finish(int e_code)
{
    mmx::logs::logI("@\n%s Ver=%d.%d.%s Exit with code = %d!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS, e_code);
    mmx::logs::log_init();
    exit(e_code);
}

void sig_handler(int sig)
{
    int e_code = -1;

    switch (sig)
    {
        case SIGPIPE:

            mmx::logs::logE("SIGPIPE");

            break;
        case SIGIO:

            mmx::logs::logE("SIGIO");

            break;
        case SIGTERM:
        case SIGQUIT:

            mmx::logs::logC("Close by terminal");

            e_code = 1;

            break;

        case SIGSEGV:

            e_code = 2;

            mmx::logs::logC("@SIGSEGV");

            break;
        default:

            e_code = 3;

            break;
    }

    if (e_code >= 0)
    {
        finish(e_code);
    }
}

int main(int argc, char* argv[])
{

    mmxlst::LISTENER_CONFIG lst_config;

    mmx::net::PortSet ports;

    lst_config.address = 0;
    lst_config.channel = 1;
    lst_config.interval = 2000;

    int rc = parse_args(argc, argv, lst_config, ports, g_log_level);

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

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Started!\nArguments: %s\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS, log_file);

        signal(SIGPIPE, sig_handler);
        signal(SIGTERM, sig_handler);
        signal(SIGQUIT, sig_handler);
        signal(SIGSEGV, sig_handler);

        mmxlst::Listener listener(lst_config, ports);

        rc = listener.Execute();

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }

    return rc;
}

int parse_ports(char* params, mmx::net::PortSet& ports)
{

        int rc = 0;

        unsigned short port = 0;
        bool range = false;
        short step = 0;

        while (params != nullptr && *params != '\0')
        {

            int num = std::atoi(params);


            if (num > 0 && num < 65536)
            {
                // задали диапазон

                if (range)
                {

                    rc += ports.SetRange(num, port, step);

                    port = num;

                    range = false;

                }
                else
                {
                    if (rc > 0)
                    {
                        step = (short)(num - port);
                    }

                    ports.Set((unsigned short)(port = num));

                    rc++;

                }

                char* p1 = std::strchr(params, ',');
                char* p2 = std::strchr(params, '-');

                if (p1 == nullptr)
                {
                    p1 = p2;
                }
                else if (p2 == nullptr)
                {
                    p2 = p1;
                }

                params = std::min(p1, p2);

                if (params != nullptr)
                {

                    range = *params++ == '-';

                }

            }
            else
            {
                rc = -EINVAL;
                break;
            }
        }


    return rc;
}

int parse_args(int argc, char* argv[], mmxlst::LISTENER_CONFIG& config, mmx::net::PortSet& ports, mmx::logs::log_level_t& log_level)
{
    int rc = 0;

    char* p;

    for(int arg = 1; arg < argc && rc == 0; ++arg)
    {
        p = argv[arg];
        if (*p == 0)
        {
            continue;
        }
        if (*p == '-')
        {
            switch(*++p)
            {
                case 'v':

                    std::cout << "Group service: " << SERVICE_GROUP << "." << std::endl
                            << "Service name: " << SERVICE_NAME << "." << std::endl
                            << "Version: " << SERVICE_MAJOR_VERSION << "." << SERVICE_MINOR_VERSION << SERVICE_STATUS << std::endl;
                    rc = 1;

                    break;
                case 'h':

                    std::cout << "!!!HELP!!!!" << std::endl;
                    rc = 1;

                    break;
                case 'c':
                    {

                        int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                        if (n > 0 && n < 255)
                        {
                            config.channel = (unsigned char)n;
                        }
                        else
                        {
                            std::cout << "Error channel number \'c=" << n << "\'. Channel number range [1..255]" << std::endl;
                            rc = -EINVAL;
                        }

                    }
                break;
                case 'i':
                {
                    int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                    if (n > 0 && n < 60000)
                    {
                        config.interval = n;
                    }
                    else
                    {
                        std::cout << "Error interval period \'c=" << n << "\'. Interval period must be range [1..60000]" << std::endl;
                        rc = -EINVAL;
                    }
                }
                break;
                case 'p':
                    {
                        if (parse_ports(*(p+1) != 0 ? p+1 : argv[++arg], ports) < 0)
                        {
                            rc = -EINVAL;
                        }

                    }
                    break;
                case 'l':
                {
                    int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                    if (n >= mmx::logs::L_TRACE && n < mmx::logs::L_CRITICAL)
                    {
                        log_level = (mmx::logs::log_level_t)n;
                    }
                    else
                    {
                        std::cout << "Error log level \'l=" << n << "\'. Log level range [" << mmx::logs::L_TRACE << ".." << mmx::logs::L_CRITICAL << "]." << std::endl;
                        rc = -EINVAL;
                    }

                }
                break;
                case 'n':
                {
                    mmx::net::address_t a = mmx::net::Socket::StoA(*(p+1) != 0 ? p+1 : argv[++arg]);

                    if (a != INADDR_NONE)
                    {
                        config.address = a;
                    }
                    else
                    {
                        std::cout << "Error input ip-address" << std::endl;
                        rc = -EINVAL;
                    }

                }
                break;

                default:
                {
                    std::cout << "Argument -" << *p << " wrong!" << std::endl;
                    rc = -EINVAL;
                }
            }
        }
        else
        {
            std::cout << "Arguments must begin with the prefix \'-\'" << std::endl;
            rc = -EINVAL;
        }
    }

    return rc;
}
