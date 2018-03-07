#ifndef _OL_SOCKET_ABSTRACT_CONNECTION_H
#define _OL_SOCKET_ABSTRACT_CONNECTION_H

//stl
#include <list>

//osnovalab
#include "OsnovaLab/Sockets/Connection.h"

namespace OsnovaLab
{
	namespace Sockets
	{
			class AbstractConnection : public IConnection
			{
			
			public:
				//IConnection
                AbstractConnection(SocketPtr socket);
				virtual ~AbstractConnection();
                virtual socket_result_t Reject(IClient* client);
                virtual const SocketPtr& GetHandle() const;
				//virtual IClient* Accept();

			protected:

                SocketPtr               socket_;
				std::list<IClient*>		clients_;

                //TODO:
				virtual socket_result_t addClient(IClient* client);
				virtual socket_result_t removeClient(IClient* client);
				virtual IClient* getClient(socket_size_t index);
				virtual bool containClient(IClient* client);
			};
	}
}

#endif
