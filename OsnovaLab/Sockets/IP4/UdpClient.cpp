#include "OsnovaLab/Sockets/IP4/UdpClient.h"
#include "OsnovaLab/Sockets/socket_wrp.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			UdpClient::UdpClient(SocketPtr& socket, const IP4Address& address) :
				address_(address),
				socket_(socket)
			{

			}

			UdpClient::~UdpClient()
			{

			}

			client_size_t UdpClient::Send(const client_data_ptr_t data, client_size_t size)
			{
				return address_.Address == SOCKET_ADDRESS_NONE
					? OsnovaLab::Sockets::Send(*socket_, data, size)
					: OsnovaLab::Sockets::IP4::SendTo(*socket_, data, size, address_);	
			}

			client_size_t UdpClient::Recv(client_data_ptr_t data, client_size_t size)
			{
				IP4Address addr;
				return address_.Address == SOCKET_ADDRESS_NONE
					? OsnovaLab::Sockets::Recv(*socket_, data, size)
					: OsnovaLab::Sockets::IP4::RecvFrom(*socket_, data, size, &addr);
			}

			//TODO: костыль
			client_size_t UdpClient::Wait(socket_timeout_t timeout, WaitMode mode)
			{
				client_size_t rc = 0;

				/*if (mode == DEFAULT)
				{
					mode = DEFAULT_MODE;
				}

				SocketSet read = { *socket_ }, write = { *socket_ };

				rc = Select(0, 
					(mode & (READ)) != 0 ? &read : nullptr,
					(mode & (WRITE)) != 0 ? &write : nullptr,
					nullptr, 
					timeout);*/

				return rc;
			}

			const SocketPtr& UdpClient::GetHandle() const
			{
				return socket_;
			}

		}
	}
}
