#include "OsnovaLab/Sockets/IP4/SocketBuilder.h"
#include "OsnovaLab/Sockets/socket_wrp.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace IP4
		{

			SocketPtr SocketBuilder::CreateTcpServerSocket(const IP4Address& address, socket_size_t backlog)
			{
                native_socket_t rc = CreateSocket(SOCKET_TCP);

                if (IsSuccess(rc))
                {
                    if (address.Address != SOCKET_ADDRESS_NONE)
                    {
                        if (!IsSuccess(Bind(rc, address)))
                        {
                            CloseSocket(rc);
                            rc = SOCKET_ERROR;
                        }
                        Listen(rc, backlog);
                    }

                }

                return MakeSocket(rc);
			}

			SocketPtr SocketBuilder::CreateUdpServerSocket(const IP4Address& address)
			{
				native_socket_t rc = CreateSocket(SOCKET_UDP);

				if (IsSuccess(rc))
				{
					if (address.Address != SOCKET_ADDRESS_NONE)
					{
						if (!IsSuccess(Bind(rc, address)))
						{
							CloseSocket(rc);
							rc = SOCKET_ERROR;
						}
					}

				}

				return MakeSocket(rc);
			}

			SocketPtr SocketBuilder::CreateTcpClientSocket(const IP4Address& address)
			{
                native_socket_t rc = CreateSocket(SOCKET_TCP);

                if (IsSuccess(rc))
                {
                    if (!IsSuccess(Connect(rc, address)))
                    {
                        CloseSocket(rc);
                        rc = SOCKET_ERROR;
                    }
                }

                return MakeSocket(rc);
			}

			SocketPtr SocketBuilder::CreateUdpClientSocket(const IP4Address& address)
			{
				native_socket_t rc = CreateSocket(SOCKET_UDP);

				if (IsSuccess(rc))
				{
					if (!IsSuccess(Connect(rc, address)))
					{
						CloseSocket(rc);
						rc = SOCKET_ERROR;
					}
				}

				return MakeSocket(rc);
			}
		}
	}
}
