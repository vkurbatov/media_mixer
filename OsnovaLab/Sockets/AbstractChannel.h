#ifndef _OL_SOCKET_ABSTRACT_CHANNEL_H
#define _OL_SOCKET_ABSTRACT_CHANNEL_H

#include <list>

#include "OsnovaLab/Sockets/Channel.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		class AbstractChannel: public IChannel
		{
		public:
			AbstractChannel();
			virtual ~AbstractChannel();
			//IChannel
			virtual IConnection* operator[] (socket_size_t index) ;
			virtual const socket_size_t Count() const;


			/*virtual socket_result_t Open(const IConnectionString& connectionString);
			virtual socket_result_t Close();
			virtual const bool IsOpened() const;

			//this
			virtual const SocketType GetChannelType() const;
			virtual const Socket& GetSocket() const;*/
		

			
		protected:
			
			std::list<IConnection*>			channels_;

			virtual socket_result_t addChannel(IConnection* server);
			virtual socket_result_t removeChannel(IConnection* server);
			virtual IConnection* getChannel(socket_size_t index);
			virtual bool containChannel(IConnection* server);

		};
	}
}

#endif
