#include "OsnovaLab/Sockets/RawConnection.h"
#include "OsnovaLab/Sockets/RawClient.h"

namespace OsnovaLab
{
	namespace Sockets
	{
        RawConnection::RawConnection(SocketPtr& socket) :
           AbstractConnection(socket)
		{

		}

		RawConnection::~RawConnection()
		{

		}

		IClient* RawConnection::Accept(const IConnectionString& connectionString)
		{
                //TODO:

			IClient* rc = new RawClient(socket_);

			if (rc != nullptr)
				addClient(rc);

			return rc;
		}
	}
}
