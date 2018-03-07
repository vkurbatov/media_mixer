#include <memory>
#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "OsnovaLab/Sockets/socket_wrp.h"

#ifdef WIN32

#pragma comment(lib,"wsock32.lib")

bool	g_sock_init = false;

#define SOCK_START() \
	if (g_sock_init == false) { \
		WSADATA wsadata; \
		WSAStartup(0x202, &wsadata); \
		g_sock_init = true; }

#define SOCKET_CLOSE(s)	::closesocket(s)

#else

#include <errno.h>

#define SOCK_START() 
#define SOCKET_CLOSE(s)	::close(s)

#endif

#define AS_FDS(ss) ((ss) != nullptr ? &(ss)->GetNativeSet() : NULL)

namespace OsnovaLab
{
	namespace Sockets
	{

		native_socket_t CreateSocket(SocketType type, int proto)
		{
			SOCK_START();
			return ::socket(DOMAIN_DEFAULT, type, proto);
		}

		socket_result_t CloseSocket(native_socket_t socket)
		{
			return SOCKET_CLOSE(socket);
		}

		socket_result_t Listen(native_socket_t socket, socket_size_t backlog)
		{
			return ::listen(socket, backlog);
		}

        socket_size_t __io_result_helper(const ssize_t& io_result)
        {
            socket_size_t rc = io_result;
            // это еще не значит что ошибка
            if (io_result < 0)
            {
                switch (errno)
                {
                    case EWOULDBLOCK:
                        rc = DEFAULT_E_UNBLOCK;
                    break;
                    case EBADF:
                        rc = DEFAULT_E_HANDLE;
                    break;
                    default:
                        rc = DEFAULT_E_SYSCALL - errno;
                    break;

                }
            }
            return rc;
        }

		socket_size_t Send(native_socket_t socket, 
			const packet_data_t data,
			socket_size_t size)
		{
            return __io_result_helper(::send(socket, (const char*)data, size, 0));
		}

		socket_size_t Recv(native_socket_t socket, 
			packet_data_t data,
			socket_size_t size)
		{
            return __io_result_helper(::recv(socket, data, size, 0));
		}

		socket_result_t Mode(native_socket_t socket, SocketBlocketType blocket_type)
		{
			socket_result_t rc = -1;

#ifdef WIN32

			u_long block = blocket_type == SOCKET_UNBLOCKED;
			rc = ::ioctlsocket(socket, FIONBIO, &block);
#else

            int flags = fcntl(socket, F_GETFL, 0);
			flags = blocket_type == SOCKET_BLOCKED ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
            rc = fcntl(socket, F_SETFL, flags);

#endif

			return rc;
		}

		namespace IP4
		{
			socket_result_t Bind(native_socket_t socket, const IP4Address address)
			{
				::sockaddr_in sin;
				sin.sin_family = DOMAIN_DEFAULT;
                sin.sin_addr.s_addr = htonl(address.Address);
				sin.sin_port = htons(address.Port);

				return ::bind(socket, (::sockaddr*)&sin, sizeof(sin));
			}

			native_socket_t Accept(native_socket_t socket, IP4Address* result)
			{
				native_socket_t rc = SOCKET_E_ERROR;

				::sockaddr_in sin;

                socklen_t len;


				std::memset(&sin, 0, sizeof(sin));
				sin.sin_family = DOMAIN_DEFAULT;
				len = sizeof(sin);

				rc = ::accept(socket, (::sockaddr*)&sin, &len);

				if ((int)rc > 0 && result != nullptr && len == sizeof(sin))
				{
                    result->Address = htonl(sin.sin_addr.s_addr);
					result->Port = htons(sin.sin_port);
				}

				return rc;
			}

			socket_result_t Connect(native_socket_t socket, const IP4Address address)
			{
				socket_result_t rc = SOCKET_E_ERROR;
				::sockaddr_in sin;

				sin.sin_family = DOMAIN_DEFAULT;
                sin.sin_addr.s_addr = htonl(address.Address);
				sin.sin_port = htons(address.Port);

                rc = __io_result_helper(connect(socket, (::sockaddr*)&sin, sizeof(sin)));

				return rc;
			}

			socket_size_t SendTo(native_socket_t socket,
				const packet_data_t data,
				socket_size_t size,
				const IP4Address address)
			{
				::sockaddr_in sin;			

				sin.sin_family = DOMAIN_DEFAULT;
                sin.sin_addr.s_addr = htonl(address.Address);
				sin.sin_port = htons(address.Port);

                return __io_result_helper(sendto(socket, (const char*)data, size, 0, (sockaddr*)&sin, sizeof(sin)));
			}

			socket_size_t RecvFrom(native_socket_t socket,
				packet_data_t data,
				socket_size_t size,
				IP4Address* result)
			{
				socket_size_t rc = SOCKET_E_ERROR;
				::sockaddr_in sin;

				sin.sin_family = DOMAIN_DEFAULT;
                socklen_t len = sizeof(sin);

                rc = __io_result_helper(recvfrom(socket, (char*)data, size, 0, (::sockaddr*)&sin, &len));

				if ((int)rc > 0 && result != nullptr && len == sizeof(sin))
				{
                    result->Address = htonl(sin.sin_addr.s_addr);
					result->Port = htons(sin.sin_port);
				}

				return rc;
			}
		}

	}
}
