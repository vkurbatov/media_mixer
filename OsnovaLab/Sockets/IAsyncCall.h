#ifndef _OL_SOCKET_I_ASYNC_CALL_H
#define _OL_SOCKET_I_ASYNC_CALL_H

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/Client.h"

namespace OsnovaLab
{
	namespace Sockets
    {

		class IAsyncCall
		{

		public:

            enum AsyncEvents
            {
                READ,
                WRITE,
                CONNECT,
                DISCONNECT,
                ERROR
            };

			virtual ~IAsyncCall() {}
            virtual socket_size_t CallRead(IClient* client, packet_data_t data, socket_size_t size, void* context) = 0;
            virtual socket_size_t CallWrite(IClient* client, packet_data_t data, socket_size_t size, void* context) = 0;
            virtual socket_size_t CallEvents(IClient* client, const AsyncEvents& event, void* context) = 0;
		};
	}
}

#endif
