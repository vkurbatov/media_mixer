#include "OsnovaLab/Sockets/SocketDispatcher.h"

namespace OsnovaLab
{
    namespace Sockets
    {
        SocketDispatcher::SocketDispatcher(IChannel& channel, IAsyncCall& async) :
            channel_(channel),
            async_(async)
            //queue_()
        {

        }

        SocketDispatcher::~SocketDispatcher()
        {

        }

        socket_result_t SocketDispatcher::DoDispatch(SocketDispatchState dispatchState)
        {

        }

        //IClient
        client_size_t SocketDispatcher::Send(const client_data_ptr_t data, client_size_t size)
        {

        }

        client_size_t SocketDispatcher::Recv(client_data_ptr_t data, client_size_t size)
        {

        }

        const SocketPtr& SocketDispatcher::GetHandle() const
        {

        }

        socket_result_t SocketDispatcher::Connect(const IConnectionString& connectionString)
        {

        }

        socket_result_t SocketDispatcher::Disconnect()
        {

        }

        socket_result_t SocketDispatcher::Reset()
        {

        }

        const SocketDispatcher::SocketReadyState SocketDispatcher::State() const
        {

        }
    }
}
