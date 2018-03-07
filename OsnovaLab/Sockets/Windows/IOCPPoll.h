#ifndef _OL_SOCKET_IOCP_POLL_H
#define _OL_SOCKET_IOCP_POLL_H

#ifdef WIN32

//stl
#include <memory>
#include <map>

//winapi
#include <windows.h>

//osnovalab
#include "OsnovaLab\Sockets\Poll.h"
#include "OsnovaLab\Data\Queue.h"

namespace OsnovaLab
{
	namespace Sockets
	{


		class IOCPPoll: public IPoll
		{


		public:

			static const size_t DEFAULT_POLL_SOCKET_CAPACITY = 200;

			//IPoll
			IOCPPoll(size_t capacity = DEFAULT_POLL_SOCKET_CAPACITY);

			virtual ~IOCPPoll() override;
			virtual socket_result_t AddSocket(SocketPtr socket, PollMode mode) override;
			virtual socket_result_t RemoveSocket(SocketPtr socket) override;
			virtual socket_result_t GetMode(SocketPtr socket, PollMode & pollMode) override;
			virtual socket_result_t SetMode(SocketPtr socket, const PollMode & pollMode) override;
			virtual socket_size_t Wait(socket_timeout_t timeout) override;
			virtual socket_size_t GetQueueEventResult(SocketPtr socket, PollMode& pollMode) override;

		};
	}
}

//WIN32
#endif

#endif
