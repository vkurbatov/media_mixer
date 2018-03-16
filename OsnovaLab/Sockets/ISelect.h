#ifndef _OL_SOCKET_I_SELECT_H
#define _OL_SOCKET_I_SELECT_H

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		//static const SocketEventMask EVENT_MASK_READWRITE = 3;
 
		class ISelect
		{
		public:
            virtual ~ISelect() {};
            //virtual socket_result_t AddSocket(const SocketPtr& socket, const event_mask_t& eventMask) = 0;
            //virtual socket_result_t RemoveSocket(const SocketPtr& socket) = 0;          
            virtual const event_mask_t GetEventMask(const SocketPtr& socket) const = 0;
            virtual socket_result_t SetEventMask(const SocketPtr& socket, const event_mask_t& eventMask = EV_NONE) = 0;
            virtual const event_mask_t operator[] (const SocketPtr& socket) = 0;
            //virtual const event_mask_t GetActiveEvents(const SocketPtr& socket) const = 0;
			virtual socket_result_t Wait(const socket_timeout_t& timeout = TIMEOUT_INFINITE) = 0;
            //virtual socket_result_t GetResult(SocketPtr& socket, event_mask_t& eventMask) = 0;
		};
	}
}

#endif
