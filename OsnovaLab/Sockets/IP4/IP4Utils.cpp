
//osnovalab
#include "OsnovaLab/Sockets/IP4/IP4Utils.h"
#include "OsnovaLab/Utils/Strings/StringUtils.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{

			std::string IP4Utils::ToString(const IP4Address& address)
			{
				return std::move(Utils::Strings::StringFormat("%02d.%02d.%02d.%02d:%d",
					address.AddressSeg[3],
					address.AddressSeg[2],
					address.AddressSeg[1],
					address.AddressSeg[0],
					address.Port)
					);
			}

			std::string IP4Utils::ToString(const address_type& address)
			{
				return std::move(Utils::Strings::StringFormat("%02d.%02d.%02d.%02d",
					((char*)&address)[3],
					((char*)&address)[2],
					((char*)&address)[1],
					((char*)&address)[0])
					);
			}

			IP4Address IP4Utils::ToAddressPort(const std::string& addressPort)
			{

			}

			address_type IP4Utils::ToAddress(const std::string& hosts)
			{

			}

			size_t IP4Utils::GetHosts(std::iostream& hosts)
			{

			}
		}
	}
}
