#include <iostream>

#include <cstring>
#include <signal.h> // sigpipe

#include "mmxlib/logs/log.h"


#include "mux.h"

#include <fcntl.h>
#include <netdb.h>  //INADDR_NONE

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Media stream multiplexor (core)"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_STATUS "debug"

#define LOG_NAME_PATTERN "/var/log/mmx/mediamux-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

#define DEFAULT_BASE_PORT       5200
#define DEFAULT_INTERVAL        2000

static char buff[1600 * 10];

static int parse_channels(char* params, unsigned char* channels);
static int parse_args(int argc, char* argv[], mmxmux::MUX_CONFIG& config, mmx::logs::log_level_t& log_level);

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
    mmxmux::MUX_CONFIG config;
    mmx::logs::log_level_t log_level = DEFAULT_LEVEL_LOG;

    std::memset(&config, 0, sizeof(config));

    config.channel_num = 1;
    config.interval = DEFAULT_INTERVAL;
    config.media_period = 20;
    config.sgm_address = INADDR_ANY;
    config.sgm_port = DEFAULT_BASE_PORT;
    //config.channels[0] |= 1 << 1;


    int rc = parse_args(argc, argv, config, log_level);

    if (rc == 0)
    {

        char log_file[256];

        std::sprintf(log_file, LOG_NAME_PATTERN, config.channel_num);

        mmx::logs::log_init(log_file, DEFAULT_LEVEL_LOG, false);

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Started!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        signal(SIGPIPE, sig_handler);
        signal(SIGTERM, sig_handler);
        signal(SIGQUIT, sig_handler);
        signal(SIGSEGV, sig_handler);

        mmxmux::Mux Mux(config);

        rc = Mux.Execute();

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }

    mmx::logs::log_init();

    return 0;
}


int parse_channels(char* params, unsigned char* channels)
{

        int rc = 0;

        unsigned char channel = 0;
        bool range = false;
        //short step = 0;

        while (params != nullptr && *params != '\0')
        {

            int num = std::atoi(params);


            if (num > 0 && num < 256)
            {
                // задали диапазон

                if (range)
                {

                    do
                    {

                        channels[channel / 8] |= 1 << (channel % 8);

                    }while(++channel <= num);

                    channel = num;

                    range = false;

                }
                else
                {

                    channel = num;

                    channels[channel / 8] |= 1 << (channel % 8);

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

int parse_args(int argc, char* argv[], mmxmux::MUX_CONFIG& config, mmx::logs::log_level_t& log_level)
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
                            config.channel_num = (unsigned char)n;
                        }
                        else
                        {
                            std::cout << "Error channel number \'c=" << n << "\'. Channel number must be range [1..255]" << std::endl;
                            rc = -EINVAL;
                        }

                    }
                    break;
                case 'o':
                    {
                        if (parse_channels(*(p+1) != 0 ? p+1 : argv[++arg], config.channels) < 0)
                        {
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
                case 'm':
                    {
                        int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                        if (n > 0 && n < 60000)
                        {
                            config.media_period = n;
                        }
                        else
                        {
                            std::cout << "Error media period \'c=" << n << "\'. Media period must be range [1..60000]" << std::endl;
                            rc = -EINVAL;
                        }
                    }
                    break;
                case 'p':
                    {
                        int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                        if (n > 0 && n < 65535)
                        {
                            config.sgm_port = (unsigned short)n;
                        }
                        else
                        {
                            std::cout << "Error port number \'c=" << n << "\'. Port number must be range [1..65534]" << std::endl;
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
                        config.sgm_address = a;
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
