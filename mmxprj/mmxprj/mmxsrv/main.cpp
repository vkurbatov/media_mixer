#include <iostream>

#include <cstring>

#include "mmxlib/logs/log.h"
#include "server.h"

#include <netdb.h>      // INADDR_ANY
#include <signal.h>

#include "../build_number.h"

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Communication dispatcher"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_BUILD_VERSION BUILD_NUMBER

#define BUILD_TIME  __TIMESTAMP__

#ifdef DEBUG
#define SERVICE_STATUS "debug"
#else
#define SERVICE_STATUS "release"
#endif

#define LOG_NAME_PATTERN "/var/log/mmx/server-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

#define DEFAULT_BASE_PORT       5400
#define DEFAULT_INTERVAL        2000

static int parse_args(int argc, char* argv[], mmxsrv::SERVER_CONFIG& config, mmx::logs::log_level_t& log_level);

void finish(int e_code)
{
    mmx::logs::logI("@%s Ver=%d.%d.%d.%s Exit with code = %d!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_STATUS, e_code);
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

    mmxsrv::SERVER_CONFIG config;

    std::memset(&config, 0, sizeof(config));

    config.channel = 1;
    config.interval = DEFAULT_INTERVAL;
    config.address = INADDR_ANY;
    config.port = DEFAULT_BASE_PORT;
    config.pult = true;

    mmx::logs::log_level_t  log_level = DEFAULT_LEVEL_LOG;

    int rc = parse_args(argc, argv, config, log_level);

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

        signal(SIGHUP, sig_handler);
        signal(SIGPIPE, sig_handler);
        signal(SIGTERM, sig_handler);
        signal(SIGQUIT, sig_handler);
        signal(SIGSEGV, sig_handler);

        rc = server.Execute();

        mmx::logs::logI("@%s Ver=%d.%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }
    return 0;
}

int parse_args(int argc, char* argv[], mmxsrv::SERVER_CONFIG& config, mmx::logs::log_level_t& log_level)
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
                            << "Version: " << SERVICE_MAJOR_VERSION << "." << SERVICE_MINOR_VERSION << "."
                            << SERVICE_BUILD_VERSION << "." << SERVICE_STATUS << std::endl;
                    rc = 1;

                    break;
                case 'h':

                    std::cout << "!!!HELP!!!!" << std::endl;

                    rc = 1;

                    break;
                case 'r':
                    {

                        int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                        if (n > 0 && n < 255)
                        {
                            config.channel = (unsigned char)n;
                        }
                        else
                        {
                            std::cout << "Error channel number \'r=" << n << "\'. Channel number must be range [1..255]" << std::endl;
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
                            std::cout << "Error interval period \'i=" << n << "\'. Interval period must be range [1..60000]" << std::endl;
                            rc = -EINVAL;
                        }
                    }
                    break;
                case 'p':
                    {
                        int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                        if (n > 0 && n < 65535)
                        {
                            config.port = (unsigned short)n;
                        }
                        else
                        {
                            std::cout << "Error port number \'p=" << n << "\'. Port number must be range [1..65534]" << std::endl;
                            rc = -EINVAL;
                        }
                    }
                    break;
                case 'd':
                {
                    int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                    if (n >= mmx::logs::L_TRACE && n < mmx::logs::L_CRITICAL)
                    {
                        log_level = (mmx::logs::log_level_t)n;
                    }
                    else
                    {
                        std::cout << "Error log level \'d=" << n << "\'. Log level range [" << mmx::logs::L_TRACE << ".." << mmx::logs::L_CRITICAL << "]." << std::endl;
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
                case 's':
                {
                    config.pult = false;

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

