#include <algorithm>

#include "OsnovaLab/Sockets/IP4/IP4AbstractChannel.h"
#include "OsnovaLab/Sockets/IP4/IP4ConnectionString.h"
#include "OsnovaLab/Sockets/Socket.h"

//#include "OsnovaLab\Sockets\socket_wrp.h"

#include "OsnovaLab/Sockets/IP4/SocketBuilder.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			IP4AbstractChannel::IP4AbstractChannel(socket_size_t capacity) :
				AbstractChannel(),
				capacity_(capacity)
			{

			}

			IP4AbstractChannel::~IP4AbstractChannel()
			{

			}

			IConnection* IP4AbstractChannel::Open(const IConnectionString& connectionString)
			{
				IConnection* rc = nullptr;

				if (Count() < capacity_)
				{
					if (IsSuccess(connectionString.QueryParam(IP4ConnectionString::FULL_ADDRESS_PARAM_ID, &address_)))
					{
						rc = createConnection(address_);
						if (rc != nullptr)
						{
							addChannel(rc);
						}				
					}
				}

				return rc;
			}

			socket_result_t IP4AbstractChannel::Close(IConnection* server)
			{
				return removeChannel(server);
			}

			const IP4Address& IP4AbstractChannel::GetAddress() const
			{
				return address_;
			}
		}
	}
}
