#ifndef _OL_SOCKET_IP4_TCP_SERVER_CONNECTION_H
#define _OL_SOCKET_IP4_TCP_SERVER_CONNECTION_H

//osnovalab
#include "OsnovaLab/Sockets/AbstractConnection.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
            class TcpServerConnection : public AbstractConnection
			{
				IP4Address		address_;
				bool			isserver_;
			public:
                TcpServerConnection(SocketPtr& socket, const IP4Address& address);

				//IServer
                virtual ~TcpServerConnection();
				virtual IClient* Accept(const IConnectionString& connectionString);
			};
		}
	}
}

#endif
