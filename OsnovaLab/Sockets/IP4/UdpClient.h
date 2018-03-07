#ifndef _OL_SOCKET_IP4_UDP_CLIENT_H
#define _OL_SOCKET_IP4_UDP_CLIENT_H

#include "OsnovaLab/Sockets/Client.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			class UdpClient : public IClient
			{
				IP4Address				address_;
				SocketPtr				socket_;
			public:
				UdpClient(SocketPtr& socket, const IP4Address& address);
				virtual ~UdpClient();
				virtual client_size_t Send(const client_data_ptr_t data, client_size_t size);
				virtual client_size_t Recv(client_data_ptr_t data, client_size_t size);
				virtual client_size_t Wait(socket_timeout_t timeout, WaitMode mode = DEFAULT);
				virtual const SocketPtr& GetHandle() const;
			};
		}
	}
}

#endif
