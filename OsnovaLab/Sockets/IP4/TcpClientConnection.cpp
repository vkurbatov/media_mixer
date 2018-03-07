#include "OsnovaLab/Sockets/IP4/TcpClientConnection.h"
#include "OsnovaLab/Sockets/IP4/TcpClient.h"
#include "OsnovaLab/Sockets/IP4/IP4ConnectionString.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
            TcpClientConnection::TcpClientConnection(SocketPtr& socket, const IP4Address& address) :
                AbstractConnection(socket),
				address_(address)
			{

			}

            TcpClientConnection::~TcpClientConnection()
			{

			}

            IClient* TcpClientConnection::Accept(const IConnectionString& connectionString)
			{
				//TODO: обернуть умными указателями
                //connectionString.QueryParam(IP4ConnectionString::FULL_ADDRESS_PARAM_ID, &addr);

                IClient* rc = new TcpClient(socket_, address_);

                if (rc != nullptr)
                    addClient(rc);

                return rc;
			}
		}
	}
}
