
#include "OsnovaLab/Sockets/Select.h"
#include "OsnovaLab/Sockets/socket_wrp.h"
#include "OsnovaLab/Thread/UniqueLock.h"

namespace OsnovaLab
{
	namespace Sockets
	{
	
		Select::Select(socket_size_t size) : waiting_(false), pulse_(false)
		{
			signal_ = MakeSocket(CreateSocket(SOCKET_UDP));
			Mode(*signal_, SOCKET_UNBLOCKED);
		}

		Select::~Select()
		{

		}

		const event_mask_t Select::GetEventMask(const SocketPtr& socket) const
		{
            if (socket != nullptr && socket->IsValid())
            {
                const native_socket_t& native_socket = *socket;

                const auto it = sockets_.find(native_socket);

                if (it != sockets_.end())
                {
                    return (*it).second.event_mask;
                    //const auto& item = sockets_[native_socket];

                    //return sockets_[native_socket].event_mask;
                }
            }
            return EV_NONE;
		}

		socket_result_t Select::SetEventMask(const SocketPtr& socket, const event_mask_t& eventMask)
		{
            socket_result_t rc = DEFAULT_E_INVARG;
            if (socket != nullptr && socket->IsValid())
            {
                const native_socket_t& native_socket = *socket;

                const auto it = sockets_.find(native_socket);

                // ситуация когда сокет есть в наборе

                if (it != sockets_.end())
                {
                    // удалить сокетный дескриптор из набора

                    if (eventMask == EV_NONE)
                    {
                        sockets_.erase(it);
                        pulse();
                    }

                    // изменить маску слежения

                    else
                    {
                        if ((*it).second.event_mask != eventMask)
                        {
                            (*it).second.event_mask = eventMask;
                            pulse();
                        }
                    }

                }

                else
                {
                    sockets_[native_socket] = { socket, eventMask, 0 };
                    pulse();
                }

                rc = sockets_.size();
            }
            return rc;
		}


		socket_result_t Select::Wait(const socket_timeout_t& timeout)
		{
			socket_result_t rc = DEFAULT_E_INVARG;

			fd_set fd_read;
			fd_set fd_write;
			fd_set fd_error;

			timeval tv = { timeout / 1000, timeout % 1000 };


			socket_size_t read_count = 1, 
				write_count = 0, 
				error_count = 0;

			FD_ZERO(&fd_read);
			FD_ZERO(&fd_write);
			FD_ZERO(&fd_error);

			FD_SET(*signal_, &fd_read);
			//FD_SET(*signal_, &fd_write);
			//FD_SET(*signal_, &fd_error);

			native_socket_t max = *signal_; 

			for (const auto& s : sockets_)
			{

				if (s.second.event_mask & EV_IOE && s.second.socket_ptr->IsValid())
				{
					if (s.first >= max)
					{
						max = s.first + 1;
					}

					if (s.second.event_mask & EV_READ)
					{
						FD_SET(s.first, &fd_read);
						read_count++;
					}
					if (s.second.event_mask & EV_WRITE)
					{
						FD_SET(s.first, &fd_write);
						write_count++;
					}
                    if (s.second.event_mask & EV_ERROR)
					{
						FD_SET(s.first, &fd_error);
						error_count++;
					}
				}
			}
		
			waiting_ = true;

			int s_rc = ::select(max,
				read_count > 0 ? &fd_read : nullptr,
				write_count > 0 ? &fd_write : nullptr,
				error_count > 0 ? &fd_error : nullptr,
				timeout != TIMEOUT_INFINITE ? &tv : nullptr);

			waiting_ = false;

			if (s_rc == 0)
			{
				rc = DEFAULT_E_TIMEOUT;
			}
			else if (s_rc < 0)
			{
				if (pulse_)
				{
					signal_ = MakeSocket(CreateSocket(SOCKET_UDP));
					rc = DEFAULT_S_SUCCESS;
				}
				else
                    rc = DEFAULT_E_SYSCALL - errno;
			}
			else
			{
                Thread::UniqueLock lock(mutex_);
				if (s_rc > 0)
				{

					for (const auto& s : sockets_)
					{
						SelectItem item;
						item.socket_ptr = s.second.socket_ptr;
						item.event_mask = EV_NONE;

						if (sockets_.find(s.first) != sockets_.end())
						{

							if (read_count > 0 && FD_ISSET(s.first, &fd_read))
							{
								item.event_mask |= EV_READ;
							}

							if (write_count > 0 && FD_ISSET(s.first, &fd_write))
							{
								item.event_mask |= EV_WRITE;
							}

							if (error_count > 0 && FD_ISSET(s.first, &fd_error))
							{
								item.event_mask |= EV_ERROR;
							}

							if (item.event_mask | EV_IOE)
							{
								queue_.push(item);

								if (--s_rc == 0)
								{
									break;
								}
							}

						}//if
					}//for
				}//if

				rc = queue_.size();
			}

			return rc;

			
		}

		socket_result_t Select::GetResult(SocketPtr& socket, event_mask_t& eventMask)
		{
            Thread::UniqueLock lock(mutex_);
			socket_result_t rc = 0;

			if (!queue_.empty())
			{
				auto item = queue_.front();
				socket = item.socket_ptr;
				eventMask = item.event_mask;
				rc = queue_.size();
				queue_.pop();
			}

			return rc;
		}

		void Select::pulse()
		{
			if (waiting_)
			{
                CloseSocket(*signal_);
				pulse_ = true;
			}
		}
	}
}
