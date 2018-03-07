#include "OsnovaLab/Sockets/IP4/TcpClient.h"
#include "OsnovaLab/Sockets/socket_wrp.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
            TcpClient::TcpClient(SocketPtr& socket, const IP4Address& address) :
                socket_(socket),
                address_(address)
			{

			}

            TcpClient::~TcpClient()
			{

			}

            client_size_t TcpClient::Send(const client_data_ptr_t data, client_size_t size)
			{
                return OsnovaLab::Sockets::Send(*socket_, data, size);
			}

            client_size_t TcpClient::Recv(client_data_ptr_t data, client_size_t size)
			{
                return OsnovaLab::Sockets::Recv(*socket_, data, size);
			}

            //TODO:
            client_size_t TcpClient::Wait(socket_timeout_t timeout, WaitMode mode)
			{
				client_size_t rc = 0;

				return rc;
			}

            const SocketPtr& TcpClient::GetHandle() const
			{
				return socket_;
			}

		}
	}
}
