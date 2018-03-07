#ifndef _OL_SOCKET_IP4_TCP_CLIENT_CONNECTION_H
#define _OL_SOCKET_IP4_TCP_CLIENT_CONNECTION_H

//osnovalab
#include "OsnovaLab/Sockets/AbstractConnection.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
            class TcpClientConnection : public AbstractConnection
			{
				IP4Address		address_;
			public:
                TcpClientConnection(SocketPtr& socket, const IP4Address& address);

				//IServer
                virtual ~TcpClientConnection();
				virtual IClient* Accept(const IConnectionString& connectionString);
			};
		}
	}
}

#endif
