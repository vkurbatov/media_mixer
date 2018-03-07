#include <memory>

#include "OsnovaLab/Sockets/Socket.h"
#include "OsnovaLab/Sockets/socket_wrp.h"

namespace OsnovaLab
{
	namespace Sockets
	{

		Socket::Socket(const native_socket_t& native_handle) : native_handle_(native_handle)
		{

		}

		Socket::Socket(Socket&& socket) : native_handle_(std::move(socket.native_handle_))
		{
			socket.native_handle_ = SOCKET_ERROR;
		}

		Socket::operator native_socket_t() const
		{
			return native_handle_;
		}

		Socket::~Socket()
		{
			if  (IsSocketValid(native_handle_))
			{
				CloseSocket(native_handle_);
			}
			native_handle_ = SOCKET_ERROR;
		}

		const bool Socket::IsValid() const
		{
			return IsSocketValid(native_handle_);
		}

		void Socket::Detach()
		{
			native_handle_ = SOCKET_ERROR;
		}

		SocketPtr MakeSocket(const native_socket_t& native_handle)
		{
			return std::make_shared<Socket>(native_handle);
			//return SocketPtr(new Socket(native_handle));
		}
	}
}

