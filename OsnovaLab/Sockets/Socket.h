#ifndef _OL_SOCKET_H
#define _OL_SOCKET_H

//stl
#include <memory> 

//osnovalab
#include "OsnovaLab/Sockets/socket_wrp.h"


namespace OsnovaLab
{
	namespace Sockets
	{

		class Socket
		{
			native_socket_t	native_handle_;

		public:

			//typedef std::shared_ptr<Socket> SocketPtr;

			Socket(const native_socket_t& native_handle);
			Socket(Socket&& socket);
			~Socket();

			const bool IsValid() const;
			operator native_socket_t() const;
			void Detach();

			/*
			Socket(Socket&& socket);
			Socket(Socket& socket);
			Socket(const native_socket_t& native_handle = SOCKET_ERROR);
			virtual ~Socket();

			operator native_socket_t() const;

			Socket& operator= (Socket&& socket);
			Socket& operator= (Socket& socket);
			const native_socket_t& operator= (const native_socket_t& native_handle);


			const native_socket_t GetNativeHandle() const;
			const bool IsValid() const;

			void Detach();
			bool Close();
			*/
		};

		using SocketPtr = std::shared_ptr<Socket>;
		SocketPtr MakeSocket(const native_socket_t& native_handle);

	}
}

#endif
