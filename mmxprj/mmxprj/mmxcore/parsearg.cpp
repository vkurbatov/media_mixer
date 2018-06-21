#include <iostream>
#include <cstdio>
#include <cstring>
#include "defines.h"

#include <netdb.h>
#include <errno.h>

#include "mmxlib/net/portset.h"

#include "mux.h"


static int parse_channels(char* params, unsigned char* channels)
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

int parse_args(int argc, char* argv[], void *cfg1, void *cfg2, mmx::logs::log_level_t &log_level)
{
	int rc = 0;

	mmxmux::MUX_CONFIG& config = *(mmxmux::MUX_CONFIG*)cfg1;

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
