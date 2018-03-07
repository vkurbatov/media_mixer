#ifndef _OL_SOCKET_I_CONNECTION_STRING
#define _OL_SOCKET_I_CONNECTION_STRING

#include <string>

#include "OsnovaLab/Sockets/SocketTypes.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		//using 
		using connection_string_result_t = default_socket_result_t;
		using connection_string_param_id_t = default_socket_size_t;
		using connection_string_param_data_t = default_socket_data_t;
		using connection_string_param_ptr_data_t = connection_string_param_data_t*;

		class IConnectionString
		{
		public:
			static const connection_string_result_t S_SUCCESS = DEFAULT_S_SUCCESS;
			static const connection_string_result_t E_INVARG = DEFAULT_E_INVARG;

            virtual ~IConnectionString() {};
			virtual const std::string GetConnectionString() = 0;
			virtual const connection_string_result_t QueryParam(connection_string_param_id_t param_id, connection_string_param_ptr_data_t data) const = 0;
			virtual const bool IsValidParamId(connection_string_param_id_t param_id) const = 0;
			virtual const bool IsEmpty() const = 0;
		};
	}
}

#endif
