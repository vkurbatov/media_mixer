#ifndef _OL_SOCKET_CLIENT_H
#define _OL_SOCKET_CLIENT_H

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{

		using client_size_t = default_socket_size_t;
		using client_data_t = default_socket_data_t;
		//using client_data_ptr_t = default_socket_ptr_data_t;
		typedef void* client_data_ptr_t;

		class IClient
		{
		public:

			enum WaitMode : int
			{
				DEFAULT = 0,
				READ = 1,
				WRITE = 2,
				BOTH = 3
			};

			const static WaitMode DEFAULT_MODE = WaitMode::READ;

            virtual ~IClient() {};
			virtual client_size_t Send(const client_data_ptr_t data, client_size_t size) = 0;
			virtual client_size_t Recv(client_data_ptr_t data, client_size_t size) = 0;
            //virtual client_size_t Wait(socket_timeout_t timeout, WaitMode mode = WaitMode::DEFAULT) = 0;
			virtual const SocketPtr& GetHandle() const = 0;
			//virtual void Detach() = 0;
		};
	}
}

#endif
