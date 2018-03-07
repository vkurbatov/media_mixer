#ifndef _OP_SOCKET_POLL_H
#define _OP_SOCKET_POLL_H

#include "OsnovaLab\Sockets\Socket.h"
//#include "OsnovaLab\Sockets\SocketTypes.h"

namespace OsnovaLab
{
	namespace Sockets
	{

		using event_mode_t = unsigned int;

		enum PollMode : event_mode_t
		{
			POLL_EVENT_NONE			= 0,
			POLL_EVENT_READ			= 1 << 0,
			POLL_EVENT_WRITE		= 1 << 1,
			POLL_EVENT_CLOSE		= 1 << 2,
			POLL_EVENT_ERROR		= 1 << 3
		};


		class IPoll
		{
		public:
			virtual ~IPoll() = 0 {};
			virtual socket_result_t AddSocket(SocketPtr socket, PollMode mode) = 0;
			virtual socket_result_t RemoveSocket(SocketPtr socket) = 0;
			virtual socket_result_t GetMode(SocketPtr socket, PollMode& pollMode) = 0;
			virtual socket_result_t SetMode(SocketPtr socket, const PollMode& pollMode) = 0;
			virtual socket_size_t Wait(socket_timeout_t timeout) = 0;
			virtual socket_size_t GetQueueEventResult(SocketPtr socket, PollMode& pollMode) = 0;

			//virtual event_mode_t WaitPoll(socket_timeout_t timeout) = 0;
		};
	}
}

#endif