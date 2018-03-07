#include <cstdlib>
#include <cstring>

#include "OsnovaLab/Sockets/IP4/IP4ConnectionString.h"
#include "OsnovaLab/Utils/Strings/StringUtils.h"


namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			IP4ConnectionString::IP4ConnectionString(const std::string& connectionString) : connectionString_(connectionString)
			{
				if (connectionString.empty() || !ToAddress(connectionString, address_))
				{
					address_.Address = SOCKET_ADDRESS_NONE;
					address_.Port = 0;
				}
			}

			IP4ConnectionString::~IP4ConnectionString()
			{

			}

			const std::string IP4ConnectionString::GetConnectionString()
			{
				//return std::sprintf
				return Utils::Strings::StringFormat(IP_FORMAT,
					address_.AddressSeg[3],
					address_.AddressSeg[2],
					address_.AddressSeg[1],
					address_.AddressSeg[0],
					address_.Port);
			}

			const connection_string_result_t IP4ConnectionString::QueryParam(connection_string_param_id_t param_id, connection_string_param_ptr_data_t data) const
			{
				connection_string_result_t rc = E_INVARG;

				switch (param_id)
				{
				case FULL_ADDRESS_PARAM_ID:
					if (data != nullptr)
					{
						std::memcpy(data, &address_, sizeof(address_));
						rc = sizeof(address_);
					}
					break;

				}

				return rc;
			}

			const bool IP4ConnectionString::IsValidParamId(connection_string_param_id_t param_id) const
			{
				return param_id == FULL_ADDRESS_PARAM_ID;
			}

			const bool IP4ConnectionString::IsEmpty() const
			{
				return connectionString_.empty();
			}

			// TODO: применить внешний парсер!!! Это костыль...
			bool IP4ConnectionString::ToAddress(const std::string& connectionString, IP4Address& address)
			{
				int state = 0;
				const char* p = connectionString.c_str();
				
				IP4Address _addr(0,0);

				while (*p != 0)
				{
					const char& chr = *p;
					switch (state)
					{
					// ip state
					case 0:
					case 1:
					case 2:
					case 3:
					{
						unsigned char &c = _addr.AddressSeg[sizeof(_addr.Address) - state - 1];
						if (chr >= '0' && chr <= '9')
							c = c * 10 + (address_type)(chr - '0');
						else if (chr == '.')
						{
							if (state < 3)
								state++;						
							else
								state = 10;
						}
						else if (chr == ':')
						{
							if (state == 3)
								state = 4;
							else
								state = 11;
						}
						else
							state = 12;
					}
						break;
					case 4:
						if (chr >= '0' && chr <= '9')
							_addr.Port = _addr.Port * 10 + (port_type)(chr - '0');
						break;
					default:
						return false;
					}

					p += (size_t)(*p != 0);
				}
				address = _addr;
				return true;
			}
		}
	}
}
