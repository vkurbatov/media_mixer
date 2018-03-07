#ifndef _OL_SOCKET_SOCKET_DISPATCHER_H
#define _OL_SOCKET_SOCKET_DISPATCHER_H

//stl
#include <set>

//osnovalab
#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/Socket.h"
#include "OsnovaLab/Sockets/IAsyncCall.h"
#include "OsnovaLab/Sockets/Channel.h"
#include "OsnovaLab/Sockets/ConnectionString.h"
#include "OsnovaLab/Data/Queue.h"
#include "OsnovaLab/Data/Packet.h"

namespace OsnovaLab
{
    namespace Sockets
    {
        class SocketDispatcher : IClient
        {
        public:
            enum SocketReadyState
            {
                READY_OPEN,
                READY_READ,
                READY_WRITE,
                READY_CLOSE
            };

            enum SocketDispatchState
            {
                DISPATCH_READ,
                DISPATCH_WRITE,
                DISPATCH_EXCEPTION
            };
        private:
            IAsyncCall &async_;
            IChannel &channel_;
            //IConnection* connection;
            //IClient* client;
            Data::Queue<Data::Packet> queue_;

            SocketReadyState    state_;
            Data::Packet pack_;

        public:

            SocketDispatcher(IChannel& channel, IAsyncCall& async);
            ~SocketDispatcher();

            socket_result_t DoDispatch(SocketDispatchState dispatchState);

            //IClient
            virtual client_size_t Send(const client_data_ptr_t data, client_size_t size);
            virtual client_size_t Recv(client_data_ptr_t data, client_size_t size);
            virtual const SocketPtr& GetHandle() const;

            virtual socket_result_t Connect(const IConnectionString& connectionString);
            virtual socket_result_t Disconnect();
            virtual socket_result_t Reset();

            const SocketReadyState State() const;

        };
    }
}

#endif
