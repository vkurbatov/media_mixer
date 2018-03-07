#ifndef _OL_SOCKET_IP4_UDP_SERVER_CHANNEL_H
#define _OL_SOCKET_IP4_UDP_SERVER_CHANNEL_H

//osnovalab
#include "OsnovaLab/Sockets/IP4/IP4AbstractChannel.h"


namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{

			class UdpServerChannel : public IP4AbstractChannel
			{

			public:

				UdpServerChannel(socket_size_t capacity = DEFAULT_MAX_CHANNEL_COUNT);

			protected:

				virtual IConnection* createConnection(const IP4Address& address);

			};
		}
	}
}

#endif
