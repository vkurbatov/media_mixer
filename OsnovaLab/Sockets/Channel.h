#ifndef _OL_SOCKET_CHANNEL_H
#define _OL_SOCKET_CHANNEL_H

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/ConnectionString.h"
#include "OsnovaLab/Sockets/Connection.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		class IChannel 
		{
		public:

			static const socket_result_t S_SUCCESS	= DEFAULT_S_SUCCESS;
			static const socket_result_t E_INVARG	= DEFAULT_E_INVARG;
			static const socket_result_t E_CONNECT	= DEFAULT_E_CONNECT;
			static const socket_result_t E_IO		= DEFAULT_E_IO;
			static const socket_result_t E_BUSY		= DEFAULT_E_BUSY;
			static const socket_result_t E_CONFIG	= DEFAULT_E_CONFIG;
			static const socket_result_t E_UNKNOWN  = DEFAULT_E_UNKNOWN;

            virtual ~IChannel() {};
			virtual IConnection* Open(const IConnectionString& connectionString) = 0;
			virtual socket_result_t Close(IConnection* server) = 0;
			virtual IConnection* operator[] (socket_size_t index) = 0;
			virtual const socket_size_t Count() const = 0;

			/*virtual socket_result_t Open(const IConnectionString& connectionString) = 0;		
			virtual socket_result_t Close() = 0;
			virtual const bool IsOpened() const = 0;*/
		};
	}
}

#endif
