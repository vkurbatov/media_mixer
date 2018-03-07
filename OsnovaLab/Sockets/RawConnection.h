#ifndef _OL_SOCKET_RAW_CONNECTION_H
#define _OL_SOCKET_RAW_CONNECTION_H

//osnovalab
#include "OsnovaLab/Sockets/AbstractConnection.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		class RawConnection : public AbstractConnection
		{
		public:
			RawConnection(SocketPtr& socket);

			//IServer
			virtual ~RawConnection();
			virtual IClient* Accept(const IConnectionString& connectionString);
		};
	}
}

#endif
