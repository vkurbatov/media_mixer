#ifdef WIN32

#include "OsnovaLab\Sockets\Windows\IOCP.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		namespace Windows
		{
			IOCP::IOCP() :
				handle_(OsnovaLab::Windows::Handle::MakeShared(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0)))
			{

			}

			IOCP::~IOCP()
			{
				while (!completions_.empty())
					completions_.pop();
				sockets_.clear();
			}

			socket_result_t IOCP::AddSocket(const native_socket_t& socket)
			{
				socket_result_t rc = DEFAULT_E_INVARG;
				if (IsSocketValid(socket))
				{
					if (sockets_.find(socket) == sockets_.end())
					{
						rc = DEFAULT_E_UNKNOWN;
						if (CreateIoCompletionPort((HANDLE)(socket), *handle_, (ULONG_PTR)socket, 0))
						{
							sockets_.insert(socket);
							rc = sockets_.size();
						}
					}
				}
				return rc;
			}

			socket_result_t IOCP::RemoveSocket(const native_socket_t& socket)
			{
				socket_result_t rc = DEFAULT_E_INVARG;

				if (sockets_.find(socket) != sockets_.end())
				{
					sockets_.erase(socket);
					rc = sockets_.size();
				}

				return rc;
			}

			socket_result_t IOCP::Wait(socket_timeout_t timeout)
			{
				socket_result_t rc = DEFAULT_E_INVARG;

				DWORD transeredBytes = 0;
				ULONG_PTR key = 0;
				LPOVERLAPPED olp = NULL;


				if (GetQueuedCompletionStatus(*handle_, &transeredBytes, &key, &olp, timeout))
				{
					if (sockets_.find((native_socket_t)key) != sockets_.end())
					{
						IOCPStatus status = { (native_socket_t)key, (socket_result_t)transeredBytes };
						completions_.push(status);
						rc = completions_.size();
					}
				}
				else
				{
					if (WSAGetLastError() == WAIT_TIMEOUT)
					{
						rc = DEFAULT_E_TIMEOUT;
					}
					else
					{
						rc = DEFAULT_E_SYSCALL;
					}
				}

				return rc;
			}

			socket_result_t IOCP::GetResult(native_socket_t& socket, socket_result_t& size)
			{
				socket_result_t rc = 0;

				if (!completions_.empty())
				{
					IOCPStatus &status = completions_.front();
					socket = status.native_socket;
					size = status.result;
					rc = completions_.size();
					completions_.pop();
				}

				return rc;
			}
		}
	}
}

#endif