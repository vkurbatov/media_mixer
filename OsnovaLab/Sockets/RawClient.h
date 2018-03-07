#ifndef _OL_SOCKET_RAW_CLIENT_H
#define _OL_SOCKET_RAW_CLIENT_H

#include "OsnovaLab/Sockets/Client.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		class RawClient : public IClient
		{
			SocketPtr				socket_;
		public:
			RawClient(SocketPtr& socket);
			virtual ~RawClient();
			virtual client_size_t Send(const client_data_ptr_t data, client_size_t size);
			virtual client_size_t Recv(client_data_ptr_t data, client_size_t size);
			virtual client_size_t Wait(socket_timeout_t timeout, WaitMode mode = DEFAULT);
			virtual const SocketPtr& GetHandle() const;
		};
	}
}

#endif
