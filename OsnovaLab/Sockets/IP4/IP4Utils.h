#ifndef _OL_SOCKET_IP4_UTILS_H
#define _OL_SOCKET_IP4_UTILS_H

//stl
#include <string>
#include <istream>

//osnovalab
#include "OsnovaLab/Sockets/SocketTypes.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			class IP4Utils
			{
			public:
				static std::string ToString(const IP4Address& address);
				static std::string ToString(const address_type& address);
				static IP4Address ToAddressPort(const std::string& addressPort);
				static address_type ToAddress(const std::string& hosts);
				static size_t GetHosts(std::iostream& hosts);

			};
		}
	}
}

#endif
