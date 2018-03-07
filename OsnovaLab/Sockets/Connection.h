#ifndef _OL_SOCKET_CONNECTION_H
#define _OL_SOCKET_CONNECTION_H

#include "OsnovaLab/Sockets/Client.h"
#include "OsnovaLab/Sockets/ConnectionString.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		//class IChannel;

		class IConnection
		{

		public:
            virtual ~IConnection() {};
            virtual IClient* Accept(const IConnectionString& connectionString) = 0;
            virtual socket_result_t Reject(IClient* client) = 0;
            virtual const SocketPtr& GetHandle() const = 0;

		};
	}
}

#endif
