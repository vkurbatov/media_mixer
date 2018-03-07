#include "OsnovaLab/Sockets/IP4/UdpServerConnection.h"
#include "OsnovaLab/Sockets/IP4/UdpClient.h"
#include "OsnovaLab/Sockets/IP4/IP4ConnectionString.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			UdpServerConnection::UdpServerConnection(SocketPtr& socket, const IP4Address& address) :
                AbstractConnection(socket),
				address_(address)
			{

			}

			UdpServerConnection::~UdpServerConnection()
			{

			}

			IClient* UdpServerConnection::Accept(const IConnectionString& connectionString)
			{
				//TODO: обернуть умными указателями
				IP4Address addr(SOCKET_ADDRESS_NONE);
				connectionString.QueryParam(IP4ConnectionString::FULL_ADDRESS_PARAM_ID, &addr);

				IClient* rc = new UdpClient(socket_, addr);
				
				if (rc != nullptr)
					addClient(rc);

				return rc;
			}
		}
	}
}
