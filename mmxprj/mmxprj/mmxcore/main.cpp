#include <iostream>

#include <cstring>
#include <signal.h> // sigpipe

#include "mmxlib/logs/log.h"


#include "mux.h"

#include <fcntl.h>
#include <netdb.h>  //INADDR_NONE

//#include "../build_number.h"

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Media stream multiplexor (core)"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_BUILD_VERSION BUILD_NUMBER

#define BUILD_TIME  __TIMESTAMP__

#ifdef DEBUG
#define SERVICE_STATUS "debug"
#else
#define SERVICE_STATUS "release"
#endif

#define LOG_NAME_PATTERN "/var/log/mmx/core-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

#define DEFAULT_BASE_PORT           5200
#define DEFAULT_INTERVAL            2000
#define DEFAULT_MIXED_GAIN          50
#define DEFAULT_MEDIA_PERIOD        20
#define DEFAULT_MUTE_TIME           200
#define DEFAULT_JITTER_BUFFER_SIZE  60
#define JITTER_BUFFER_MIN           20
#define JITTER_BUFFER_MAX           (5 * 60 * 1000)

static char buff[1600 * 10];

static int parse_channels(char* params, unsigned char* channels);
static int parse_args(int argc, char* argv[], mmxmux::MUX_CONFIG& config, mmx::logs::log_level_t& log_level);

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


   /* unsigned char b_a[] = { 0x15, 0x10, 0x02, 0x0A };
    unsigned char b_p[] = { 0xC6, 0x13 };
    auto a = mmx::net::Socket::AtoS(*(int*)b_a);
    auto p = *(unsigned short*)b_p;*/

    int rc = parse_args(argc, argv, config, log_level);

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

        signal(SIGHUP, sig_handler);
        signal(SIGPIPE, sig_handler);
        signal(SIGTERM, sig_handler);
        signal(SIGQUIT, sig_handler);
        signal(SIGSEGV, sig_handler);

        mmxmux::Mux Mux(config);

        rc = Mux.Execute();

        mmx::logs::logI("@%s Ver=%d.%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_BUILD_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }

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
                            << "Version: " << SERVICE_MAJOR_VERSION << "." << SERVICE_MINOR_VERSION << "."
                            << SERVICE_BUILD_VERSION << "." << SERVICE_STATUS << std::endl;
                    rc = 1;

                    break;
                case 'h':

                    //std::cout << "!!!HELP!!!!" << std::endl;

                    std::cout << std::endl << "usage: " << SERVICE_GROUP << "." << std::endl
                            << "-v - output to screen version info" << std::endl
                            << "-o<1,2-n> - set outpet channels, " << std::endl;

                    rc = 1;

                    break;
                case 'r':
                    {

                        int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                        if (n > 0 && n < 255)
                        {
                            config.channel_num = (unsigned char)n;
                        }
                        else
                        {
                            std::cout << "Error channel number \'r=" << n << "\'. Channel number must be range [1..255]" << std::endl;
                            rc = -EINVAL;
                        }

                    }
                    break;
                case 'w':
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
                            std::cout << "Error interval period \'i=" << n << "\'. Interval period must be range [1..60000]" << std::endl;
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
                            std::cout << "Error media period \'m=" << n << "\'. Media period must be range [1..60000]" << std::endl;
                            rc = -EINVAL;
                        }
                    }
                    break;
                case 's':
                    {
                            int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                            if (n >= 0 && n <= 30000)
                            {
                                config.mute_time = (unsigned short)n;
                            }
                            else
                            {
                                std::cout << "Error silense time \'s=" << n << "\'. Silense must be range [0..30000] msec" << std::endl;
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
                        std::cout << "Error log level \'d=" << n << "\'. Log level must be range [" << mmx::logs::L_TRACE << ".." << mmx::logs::L_CRITICAL << "]." << std::endl;
                        rc = -EINVAL;
                    }

                }
                break;
                case 'j':
                {
                    int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                    if (n >= JITTER_BUFFER_MIN && n < JITTER_BUFFER_MAX)
                    {
                        config.jitter_size = n;
                    }
                    else
                    {
                        std::cout << "Error jitter size \'j=" << n << "\'. Jitter size must be range [" << JITTER_BUFFER_MIN << ".." << JITTER_BUFFER_MAX << "]." << std::endl;
                        rc = -EINVAL;
                    }

                }
                break;
                case 'g':
                {
                    int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

                    if (n >= 0 && n < 255)
                    {
                        config.mixed_gain = n;
                    }
                    else
                    {
                        std::cout << "Error mixer gain \'g=" << n << "\'. Mixer gain must be range [0..255]." << std::endl;
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
