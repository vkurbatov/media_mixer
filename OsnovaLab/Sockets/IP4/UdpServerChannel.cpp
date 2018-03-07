#include <algorithm>

#include "OsnovaLab/Sockets/IP4/UdpServerChannel.h"
#include "OsnovaLab/Sockets/IP4/UdpServerConnection.h"
#include "OsnovaLab/Sockets/IP4/SocketBuilder.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{
			UdpServerChannel::UdpServerChannel(socket_size_t capacity) :
				IP4AbstractChannel(capacity)
			{

			}

			IConnection* UdpServerChannel::createConnection(const IP4Address& address)
			{
				SocketPtr socket = SocketBuilder::CreateUdpServerSocket(address);

				if (socket->IsValid())
				{
					return new UdpServerConnection(socket, address);
				}

				return nullptr;
			}

		}
	}
}
