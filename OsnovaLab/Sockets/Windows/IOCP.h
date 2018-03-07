#ifndef _OL_SOCKET_IOCP_H
#define _OL_SOCKET_IOCP_H

#ifdef WIN32

//stl
#include <set>
#include <queue>

//windows
//#include <windows.h>

//osnovalab
//#include "OsnovaLab\Data\Queue.h"
#include "OsnovaLab\Windows\Handle.h"
#include "OsnovaLab\Sockets\Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace Windows
		{

			class IOCP
			{
				struct IOCPStatus
				{
					native_socket_t		native_socket;
					socket_result_t		result;
				};

				OsnovaLab::Windows::Handle::HandleShared	handle_;
				std::set<native_socket_t>					sockets_;
				std::queue<IOCPStatus>						completions_;

			public:
				IOCP();
				~IOCP();
				socket_result_t AddSocket(const native_socket_t& socket);
				socket_result_t RemoveSocket(const native_socket_t& socket);
				socket_result_t Wait(socket_timeout_t timeout);
				socket_result_t GetResult(native_socket_t& socket, socket_result_t& size);
			};
		}
	}
}

//WIN32
#endif

#endif
