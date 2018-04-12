
/* Сервис-слушатель UDP портов. Читает
 *
 *
 *
 *
 */

#include <iostream>
#include <cstring>

#include <netdb.h>  // INADDR_ANY

#include "mmxlib/logs/log.h"
#include "listener.h"

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Udp media stream listener"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_STATUS "debug"

#define LOG_NAME_PATTERN "/var/log/mmx/listener-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

static int parse_ports(char* params, mmx::net::PortSet& ports);
static int parse_args(int argc, char* argv[], mmx::net::address_t& address, unsigned char& channel, mmx::net::PortSet& ports, mmx::logs::log_level_t& log_level);

int main(int argc, char* argv[])
{


    mmx::net::PortSet ports;

    mmx::logs::log_level_t log_level = DEFAULT_LEVEL_LOG;

    mmx::net::address_t address = 0;

    unsigned char channel = 1;

    int rc = parse_args(argc, argv, address, channel, ports, log_level);

    if (rc == 0)
    {

        char log_file[256];

        std::sprintf(log_file, LOG_NAME_PATTERN, channel);

        mmx::logs::log_init(log_file, log_level, false);

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Started!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmxlst::Listener listener(address, channel, ports);

        rc = listener.Execute();

        mmx::logs::logI("@\n%s Ver=%d.%d.%s Stopped!\n\n", SERVICE_NAME, SERVICE_MAJOR_VERSION, SERVICE_MINOR_VERSION, SERVICE_STATUS);

        mmx::logs::log_init();

    }

    return rc;
}

int parse_ports(char* params, mmx::net::PortSet& ports)
{

        int rc = 0;

        short port = 0;
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

int parse_args(int argc, char* argv[], mmx::net::address_t& address, unsigned char& channel, mmx::net::PortSet& ports, mmx::logs::log_level_t& log_level)
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
                            channel = (unsigned char)n;
                        }
                        else
                        {
                            std::cout << "Error channel number \'c=" << n << "\'. Channel number range [1..255]" << std::endl;
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
                        address = a;
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
