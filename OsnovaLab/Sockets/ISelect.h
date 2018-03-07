#ifndef _OL_SOCKET_I_SELECT_H
#define _OL_SOCKET_I_SELECT_H

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/Socket.h"

namespace OsnovaLab
{
	namespace Sockets
	{

		using event_mask_t = socket_size_t;

		static const event_mask_t EV_NONE		= 0;
		static const event_mask_t EV_READ		= 1;
		static const event_mask_t EV_WRITE		= EV_READ << 1;
		static const event_mask_t EV_ERROR		= EV_WRITE << 1;

        //TODO: Пока не реализованы
		static const event_mask_t EV_ADD		= EV_ERROR << 1;
		static const event_mask_t EV_REMOVE		= EV_ADD << 1;

		static const event_mask_t EV_IO = EV_READ | EV_WRITE;
		static const event_mask_t EV_IOE = EV_IO | EV_ERROR;
		static const event_mask_t EV_ALL = EV_IOE | EV_ADD | EV_REMOVE;


		//static const SocketEventMask EVENT_MASK_READWRITE = 3;
 
		class ISelect
		{
		public:
            virtual ~ISelect() {};
            //virtual socket_result_t AddSocket(const SocketPtr& socket, const event_mask_t& eventMask) = 0;
            //virtual socket_result_t RemoveSocket(const SocketPtr& socket) = 0;
            virtual const event_mask_t GetEventMask(const SocketPtr& socket) const = 0;
            virtual socket_result_t SetEventMask(const SocketPtr& socket, const event_mask_t& eventMask = EV_NONE) = 0;
			virtual socket_result_t Wait(const socket_timeout_t& timeout = TIMEOUT_INFINITE) = 0;
			virtual socket_result_t GetResult(SocketPtr& socket, event_mask_t& eventMask) = 0;
		};
	}
}

#endif
