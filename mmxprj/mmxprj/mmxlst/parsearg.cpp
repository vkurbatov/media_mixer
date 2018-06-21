#include <iostream>
#include <cstdio>
#include <cstring>
#include "defines.h"

#include <netdb.h>
#include <errno.h>

#include "mmxlib/net/portset.h"

#include "listener.h"

int set_range(mmx::net::PortSet& ports, int begin, int end, int step = 1)
{
	int rc = 0;

	step += (short)(step == 0);

	step = std::abs(step);

	step = begin <= end ? step : -step;

	// swap begin и end не прокатит, мы должны в люблом случае установить порт begin

	while (step > 0 ? begin <= end : begin >= end)
	{
		rc++;

		ports.Set(step);

		begin += step;
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

					rc += set_range(ports, num, port, step);
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

int parse_args(int argc, char* argv[], void *cfg1, void *cfg2, mmx::logs::log_level_t &log_level)
{
	int rc = 0;

	mmxlst::LISTENER_CONFIG& config = *(mmxlst::LISTENER_CONFIG*)cfg1;
	mmx::net::PortSet& ports = *(mmx::net::PortSet*)cfg2;

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
				case 'w':
					{

						int n = atoi(*(p+1) != 0 ? p+1 : argv[++arg]);

						if (n > 0 && n < 255)
						{
							config.channel = (unsigned char)n;
						}
						else
						{
							std::cout << "Error channel number \'w=" << n << "\'. Channel number range [1..255]" << std::endl;
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
						if (parse_ports(*(p+1) != 0 ? p+1 : argv[++arg], ports) < 0)
						{
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
