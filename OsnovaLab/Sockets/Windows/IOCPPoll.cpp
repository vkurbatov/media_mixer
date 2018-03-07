#ifdef WIN32

#include "IOCPPoll.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		IOCPPoll::IOCPPoll(size_t capacity) 
		{

		}

		IOCPPoll::~IOCPPoll()
		{

		}

		socket_result_t IOCPPoll::AddSocket(SocketPtr socket, PollMode mode)
		{
			return socket_size_t();
		}

		socket_result_t IOCPPoll::RemoveSocket(SocketPtr socket)
		{
			return socket_size_t();
		}

		socket_result_t IOCPPoll::GetMode(SocketPtr socket, PollMode & pollMode)
		{
			return socket_size_t();
		}

		socket_result_t IOCPPoll::SetMode(SocketPtr socket, const PollMode & pollMode)
		{
			return socket_size_t();
		}

		socket_size_t IOCPPoll::Wait(socket_timeout_t timeout)
		{
			return socket_size_t();
		}

		socket_size_t IOCPPoll::GetQueueEventResult(SocketPtr socket, PollMode & pollMode)
		{
			return socket_size_t();
		}

	}
}

#endif

