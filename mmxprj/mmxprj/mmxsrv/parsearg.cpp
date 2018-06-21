#include <iostream>
#include <cstdio>
#include <cstring>
#include "defines.h"

#include <netdb.h>
#include <errno.h>


#include "server.h"


int parse_args(int argc, char* argv[], void *cfg1, void *cfg2, mmx::logs::log_level_t &log_level)
{
	int rc = 0;

	char* p;

	mmxsrv::SERVER_CONFIG& config = *(mmxsrv::SERVER_CONFIG*)cfg1;

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
					if (*(p+1) == 'r')
					{
						config.mode = mmxsrv::ORM_LINK_E1_REVERSE;
					}
					else
					{
						config.mode = mmxsrv::ORM_LINK_E1;
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
