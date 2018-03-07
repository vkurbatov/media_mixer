#include "OsnovaLab/Sockets/RawClient.h"
#include "OsnovaLab/Sockets/socket_wrp.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		RawClient::RawClient(SocketPtr& socket) :
				socket_(socket)
		{

		}

		RawClient::~RawClient()
		{

		}

		client_size_t RawClient::Send(const client_data_ptr_t data, client_size_t size)
		{
			return OsnovaLab::Sockets::Send(*socket_, data, size);
		}

		client_size_t RawClient::Recv(client_data_ptr_t data, client_size_t size)
		{
			return OsnovaLab::Sockets::Recv(*socket_, data, size);
		}

			//TODO: костыль
		client_size_t RawClient::Wait(socket_timeout_t timeout, WaitMode mode)
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

		const SocketPtr& RawClient::GetHandle() const
		{
				return socket_;
		}
	}
}
