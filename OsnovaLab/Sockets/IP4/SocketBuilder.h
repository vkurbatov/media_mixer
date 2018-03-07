#ifndef _OL_SOCKET_IP4_SOCKET_BUILDER_H
#define _OL_SOCKET_IP4_SOCKET_BUILDER_H

#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			class SocketBuilder
			{
			public:
				static SocketPtr CreateTcpServerSocket(const IP4Address& address, socket_size_t backlog);
				static SocketPtr CreateUdpServerSocket(const IP4Address& address);
				static SocketPtr CreateTcpClientSocket(const IP4Address& address);
				static SocketPtr CreateUdpClientSocket(const IP4Address& address);
			};
		}
	}
}

#endif
