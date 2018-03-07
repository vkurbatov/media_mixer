#ifndef _OL_SOCKET_IP4_UDP_SERVER_CONNECTION_H
#define _OL_SOCKET_IP4_UDP_SERVER_CONNECTION_H

//osnovalab
#include "OsnovaLab/Sockets/AbstractConnection.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			class UdpServerConnection : public AbstractConnection
			{
				IP4Address		address_;
			public:
				UdpServerConnection(SocketPtr& socket, const IP4Address& address);

				//IServer
				virtual ~UdpServerConnection();
				virtual IClient* Accept(const IConnectionString& connectionString);
			};
		}
	}
}

#endif
