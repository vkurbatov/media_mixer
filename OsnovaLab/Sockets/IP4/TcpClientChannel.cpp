#include <algorithm>

#include "OsnovaLab/Sockets/IP4/TcpClientChannel.h"
#include "OsnovaLab/Sockets/IP4/TcpClientConnection.h"
#include "OsnovaLab/Sockets/IP4/SocketBuilder.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
            TcpClientChannel::TcpClientChannel(socket_size_t capacity) :
				IP4AbstractChannel(capacity)
			{

			}

            IConnection* TcpClientChannel::createConnection(const IP4Address& address)
			{
                SocketPtr socket = SocketBuilder::CreateTcpClientSocket(address);

				if (socket->IsValid())
				{
                    return new TcpClientConnection(socket, address);
				}

				return nullptr;
			}

		}
	}
}
