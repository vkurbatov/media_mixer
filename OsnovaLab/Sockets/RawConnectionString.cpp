#include "OsnovaLab/Sockets/RawConnectionString.h"


namespace OsnovaLab
{
	namespace Sockets
	{
		RawConnectionString::RawConnectionString()
		{

		}

		RawConnectionString::~RawConnectionString()
		{

		}

		const std::string RawConnectionString::GetConnectionString()
		{
				return std::string();
		}

		const connection_string_result_t RawConnectionString::QueryParam(connection_string_param_id_t param_id, connection_string_param_ptr_data_t data) const
		{
                return OsnovaLab::Sockets::DEFAULT_E_NOTIMPL;
		}

		const bool RawConnectionString::IsValidParamId(connection_string_param_id_t param_id) const
		{
				return false;
		}

		const bool RawConnectionString::IsEmpty() const
		{
				return true;
		}

	}
}
