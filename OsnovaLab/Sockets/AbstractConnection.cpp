#include <algorithm>

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/AbstractConnection.h"


namespace OsnovaLab
{
	namespace Sockets
	{
        AbstractConnection::AbstractConnection(SocketPtr socket) :
            socket_(socket)
        {

        }

		AbstractConnection::~AbstractConnection()
		{
			for (auto& c : clients_)
			{
				if (c != nullptr)
				{
					delete c;
				}
			}

			clients_.clear();
		}

        socket_result_t AbstractConnection::Reject(IClient* client)
        {
            return removeClient(client);
        }

        const SocketPtr& AbstractConnection::GetHandle() const
        {
            return socket_;
        }

		socket_result_t AbstractConnection::addClient(IClient* client)
		{
			socket_result_t rc = DEFAULT_E_INVARG;

			if (client != nullptr && !containClient(client))
			{
				clients_.push_back(client);
				rc = clients_.size();
			}

			return rc;
		}

		socket_result_t AbstractConnection::removeClient(IClient* client)
		{
			socket_result_t rc = DEFAULT_E_INVARG;

			if (client != nullptr && containClient(client))
			{
				clients_.remove(client);
				delete client;
				rc = clients_.size();
			}

			return rc;
		}

		IClient* AbstractConnection::getClient(socket_size_t index)
		{

			if (index >= 0 && index < clients_.size())
			{
				auto it = clients_.begin();
				std::advance(it, index);
				return *it;
			}
			return nullptr;
		}

		bool AbstractConnection::containClient(IClient* client)
		{
			return client != nullptr && std::find(clients_.begin(), clients_.end(), client) != clients_.end();
		}
	}
}
