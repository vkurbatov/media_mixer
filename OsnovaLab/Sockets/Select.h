#ifndef _OL_SOCKET_SELECT_H
#define _OL_SOCKET_SELECT_H

//stl
#include <map>
#include <queue>

//osnovalab
#include "OsnovaLab/Sockets/ISelect.h"
#include "OsnovaLab/Thread/StdMutex.h"

namespace OsnovaLab
{
	namespace Sockets
	{

		struct SelectItem
		{
			SocketPtr			socket_ptr;
			event_mask_t		event_mask;
            event_mask_t        active_mask;
            //socket_timeout_t	elapsed;
		};

		class Select : public ISelect
		{
			std::map<native_socket_t, SelectItem>	sockets_;
            //std::queue<SelectItem>					queue_;
			bool									waiting_;
			bool									pulse_;

            Thread::StdMutex                        mutex_;


			SocketPtr								signal_;

		public:

			static const socket_size_t SELECT_MAX_SOCKETS = SOCKET_SET_SIZE;

			Select(socket_size_t size = SELECT_MAX_SOCKETS);
			virtual ~Select() override;
            //virtual socket_result_t AddSocket(const SocketPtr& socket, const event_mask_t& eventMask) override;
            //virtual socket_result_t RemoveSocket(const SocketPtr& socket) override;
            virtual const event_mask_t GetEventMask(const SocketPtr& socket) const override;
            virtual socket_result_t SetEventMask(const SocketPtr& socket, const event_mask_t& eventMask = EV_NONE) override;
            //virtual const event_mask_t GetActiveEvents(const SocketPtr& socket) const override;
            virtual const event_mask_t operator[] (const SocketPtr& socket);
			virtual socket_result_t Wait(const socket_timeout_t& timeout = TIMEOUT_INFINITE) override;

            //virtual socket_result_t GetResult(SocketPtr& socket, event_mask_t& eventMask) override;
		private:
			void pulse();
		};
	}
}

#endif
