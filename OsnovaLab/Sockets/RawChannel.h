#ifndef _OL_SOCKET_RAW_CHANNEL_H
#define _OL_SOCKET_RAW_CHANNEL_H

#include <list>

#include "OsnovaLab/Sockets/AbstractChannel.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		class RawChannel : public AbstractChannel
		{
		public:

			RawChannel();
			
			//IChannel
			virtual IConnection* Open(const IConnectionString& connectionString);
			virtual socket_result_t Close(IConnection* server);

		};
	}
}

#endif
