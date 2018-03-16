#ifndef _OL_SOCKET_WRP_H
#define _OL_SOCKET_WRP_H

#include "OsnovaLab/Sockets/SocketTypes.h"

namespace OsnovaLab
{

	namespace Sockets
	{
		native_socket_t CreateSocket(SocketType type, int proto = 0);
		socket_result_t CloseSocket(native_socket_t socket);
		socket_result_t Listen(native_socket_t socket, socket_size_t backlog);
		socket_size_t Send(native_socket_t socket, 
			packet_data_t data,
            socket_size_t size);
		socket_size_t Recv(native_socket_t socket, 
			packet_data_t data,
			socket_size_t size);

		socket_result_t Mode(native_socket_t socket, SocketBlocketType blocket_type);

        //socket_result_t Bind_Connect_Listen();

		namespace IP4
		{
			socket_result_t Bind(native_socket_t socket, const IP4Address address);
			native_socket_t Accept(native_socket_t socket, IP4Address* result);
			socket_result_t Connect(native_socket_t socket, const IP4Address address);
			

			socket_size_t SendTo(native_socket_t socket,
				const packet_data_t data,
				socket_size_t size, 
				const IP4Address address);

			socket_size_t RecvFrom(native_socket_t socket,
				packet_data_t data,
				socket_size_t size,
				IP4Address* result);
		}
	}
}

#endif
