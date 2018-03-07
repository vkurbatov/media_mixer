#include <algorithm>

#include "OsnovaLab/Sockets/IP4/TcpServerChannel.h"
#include "OsnovaLab/Sockets/IP4/TcpServerConnection.h"
#include "OsnovaLab/Sockets/IP4/SocketBuilder.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
            TcpServerChannel::TcpServerChannel(socket_size_t capacity) :
				IP4AbstractChannel(capacity)
			{

			}

            IConnection* TcpServerChannel::createConnection(const IP4Address& address)
			{
                SocketPtr socket = SocketBuilder::CreateTcpServerSocket(address, 4);

				if (socket->IsValid())
				{
                    return new TcpServerConnection(socket, address);
				}

				return nullptr;
			}

		}
	}
}
