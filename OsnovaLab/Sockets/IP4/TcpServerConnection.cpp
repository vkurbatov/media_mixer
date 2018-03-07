#include "OsnovaLab/Sockets/IP4/TcpServerConnection.h"
#include "OsnovaLab/Sockets/IP4/TcpClient.h"
#include "OsnovaLab/Sockets/IP4/IP4ConnectionString.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
            TcpServerConnection::TcpServerConnection(SocketPtr& socket, const IP4Address& address) :
                AbstractConnection(socket),
				address_(address)
			{

			}

            TcpServerConnection::~TcpServerConnection()
			{

			}

            IClient* TcpServerConnection::Accept(const IConnectionString& connectionString)
			{
				//TODO: обернуть умными указателями
                //connectionString.QueryParam(IP4ConnectionString::FULL_ADDRESS_PARAM_ID, &addr);

                IP4Address addr;
                SocketPtr cli = MakeSocket(IP4::Accept(*socket_,&addr));
                if (cli->IsValid())
                {
                    IClient* rc = new TcpClient(cli, addr);

                    if (rc != nullptr)
                        addClient(rc);

                    return rc;
                }
                return nullptr;

			}
		}
	}
}
