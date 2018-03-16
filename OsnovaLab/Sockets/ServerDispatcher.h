#ifndef _OL_INCLUDE_SERVER_HELPER_H
#define _OL_INCLUDE_SERVER_HELPER_H

#include <map>

#include "OsnovaLab/Sockets/SocketTypes.h"
#include "OsnovaLab/Sockets/Channel.h"
#include "OsnovaLab/Sockets/IAsyncCall.h"
#include "OsnovaLab/Sockets/ISelect.h"
#include "OsnovaLab/Data/Queue.h"
#include "OsnovaLab/Data/Packet.h"

namespace OsnovaLab
{
    namespace Sockets
    {
        //TODO: данную реализацию нужно декомпозировать как минимум на позкласс направления
        // также необходимо добавить обработку уточняющей ниформации конкретного протокола (IP:PORT)
        class ServerDispatcher
        {
            enum DispatchState
            {
                INIT,
                CONNECT,
                SELECT,
                SHUTDOWN
            };

            struct Direction
            {
                IClient* client;                        // полученое соединение
                Data::Queue<Data::Packet> send_queue;   // очередь на передачу
                Data::Queue<Data::Packet> recv_queue;   // очередь на прием
            };


            IChannel& channel_;
            IConnectionString& serverString_;
            IAsyncCall& async_;
            ISelect& select_;
            IConnection* connection;

            DispatchState   state_;
            std::map<native_socket_t, Direction> dirs_;


        public:

            ServerDispatcher(IChannel& channel,
                         IConnectionString& serverString,
                         IAsyncCall& async,
                         ISelect& select);

            socket_result_t Dispatch(socket_timeout_t timeout = TIMEOUT_INFINITE);

            ~ServerDispatcher();

        protected:

            // работа с состояниями канала
            socket_result_t init();
            socket_result_t connect();
            // основное состояние канала
            socket_result_t select(socket_timeout_t timeout);
            socket_result_t shutdown();

            // работа с направлениями соединения
            socket_result_t build_direction();
            socket_result_t destroy_direction(const SocketPtr& socket);
            socket_result_t process_direction(const SocketPtr& socket, event_mask_t eventMask);

        };
    }
}

#endif
