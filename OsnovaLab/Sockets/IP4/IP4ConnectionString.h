#ifndef _OL_SOCKET_IP4_CONNECTION_STRING
#define _OL_SOCKET_IP4_CONNECTION_STRING

//stl
#include <string>

//osnovallab
#include "OsnovaLab/Sockets/ConnectionString.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			static const char* IP_FORMAT = "%d.%d.%d.%d:%d";

			class IP4ConnectionString : public IConnectionString
			{
				std::string							connectionString_;
				IP4Address							address_;
			public:
		
				static const connection_string_param_id_t	FULL_ADDRESS_PARAM_ID = 1;

				IP4ConnectionString(const std::string& connectionString);
				virtual ~IP4ConnectionString();
				virtual const std::string GetConnectionString() override;
				virtual const connection_string_result_t QueryParam(connection_string_param_id_t param_id, connection_string_param_ptr_data_t data) const override;
				virtual const bool IsValidParamId(connection_string_param_id_t param_id) const override;
				virtual const bool IsEmpty() const override;

				static bool ToAddress(const std::string& connectionString, IP4Address& address);
			};

			static const IP4ConnectionString EMPTY_STRING = IP4ConnectionString("");
		}
	}
}

#endif
