#ifndef _OL_SOCKET_IP4_ABSTRACT_CHANNEL_H
#define _OL_SOCKET_IP4_ABSTRACT_CHANNEL_H

//osnovalab
#include "OsnovaLab/Sockets/AbstractChannel.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{

			class IP4AbstractChannel : public AbstractChannel
			{
				socket_size_t				capacity_;
				IP4Address					address_;

			public:

				static const socket_size_t DEFAULT_MAX_CHANNEL_COUNT = SOCKET_SET_SIZE;

				IP4AbstractChannel(socket_size_t capacity = DEFAULT_MAX_CHANNEL_COUNT);

				//IChannel
				virtual ~IP4AbstractChannel();
				virtual IConnection* Open(const IConnectionString& connectionString);
				virtual socket_result_t Close(IConnection* server);

				virtual const IP4Address& GetAddress() const;
			
			protected:
				virtual IConnection* createConnection(const IP4Address& address) = 0;
			};
		}
	}
}

#endif
