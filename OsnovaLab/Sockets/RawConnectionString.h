#ifndef _OL_SOCKET_RAW_CONNECTION_STRING
#define _OL_SOCKET_RAW_CONNECTION_STRING

//osnovallab
#include "OsnovaLab/Sockets/ConnectionString.h"

namespace OsnovaLab
{
	namespace Sockets
	{

		class RawConnectionString : public IConnectionString
		{

		public:

			RawConnectionString();
			virtual ~RawConnectionString();
			virtual const std::string GetConnectionString() override;
			virtual const connection_string_result_t QueryParam(connection_string_param_id_t param_id, connection_string_param_ptr_data_t data) const override;
			virtual const bool IsValidParamId(connection_string_param_id_t param_id) const override;
			virtual const bool IsEmpty() const override;
		};
	}
}

#endif
