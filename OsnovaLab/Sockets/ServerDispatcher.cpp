#include "OsnovaLab/Sockets/ServerDispatcher.h"

namespace OsnovaLab
{
    namespace Sockets
    {
        ServerHelper::ServerHelper(IChannel& channel,
                     IConnectionString& serverString,
                     IAsyncCall& async,
                     ISelect& select) :
            channel_(channel),
            serverString_(serverString),
            async_(async),
            select_(select),
            state_(INIT),
            connection(nullptr)
        {

        }

        ServerHelper::~ServerHelper()
        {

        }

        socket_result_t ServerHelper::Dispatch(socket_timeout_t timeout)
        {
            socket_result_t rc = DEFAULT_E_INVARG;
            switch(state_)
            {
                case INIT:
                    {
                        rc = init();
                    }
                    break;

                case CONNECT:
                    {
                        rc = connect();
                    }
                    break;

                case SELECT:
                    {
                        rc = select(timeout);
                    }
                    break;

                case SHUTDOWN:
                    {
                        rc = shutdown();
                    }
                    break;
            }

            return rc;
        }

        socket_result_t ServerHelper::init()
        {
            // тут нужно добавить очистку направлений и селектора

            if (connection != nullptr)
            {
                // эти вещи делаются в состоянии SHUTDOWN
                select_.SetEventMask(connection->GetHandle());
                channel_.Close(connection);
                delete connection;
                connection = nullptr;

            }

            state_ = CONNECT;
            return DEFAULT_S_SUCCESS;
        }

        socket_result_t ServerHelper::connect()
        {
            if ((connection = channel_.Open(serverString_)) != nullptr)
            {
                // переводим сокет в неблокирующий режим
                Sockets::Mode(*connection->GetHandle(), SOCKET_UNBLOCKED);

                // добавляем дескриптор в набор селектора для ожидания события
                select_.SetEventMask(connection->GetHandle(), EV_READ);
                state_ = SELECT;
            }
            else
                state_ = INIT;
            return DEFAULT_S_SUCCESS;
        }

        socket_result_t ServerHelper::select(socket_timeout_t timeout)
        {
            socket_result_t rc = select_.Wait(timeout);

            if (rc > 0)
            {

                SocketPtr socket;
                event_mask_t mask;

                while (select_.GetResult(socket, mask))
                {
                    if (socket != nullptr && socket->IsValid())
                    {
                        // пришел запрос на соединение
                        if (socket == connection->GetHandle())
                        {
                            // принимаем новое соединение
                            build_direction();
                        }
                        else
                        {
                            // обрабатываем конкретное соединение
                            process_direction(socket, mask);
                        }
                    }
                }
            }
            return rc;
        }

        socket_result_t ServerHelper::shutdown()
        {
            // сперва удалим все дескрипторы с наблюдения селектора
            for (auto& d : dirs_)
            {
                select_.SetEventMask(d.second.client->GetHandle());
            }

            // затем удалим все направления

            dirs_.clear();

            // также удалим дескриптор прослушивающего сокета из селектора

            if (connection != nullptr) // такого быть не должно
            {
                select_.SetEventMask(connection->GetHandle());
                channel_.Close(connection);
                delete connection;
                connection = nullptr;
            }
        }

        socket_result_t ServerHelper::build_direction()
        {

            if (connection != nullptr)
            {
                auto client = connection->Accept(serverString_);

                if (client != nullptr)
                {
                    const auto& socket = client->GetHandle();
                    native_socket_t native_socket = *socket;

                    //переводим сокет в неблокирующий режим

                    Sockets::Mode(native_socket, SOCKET_UNBLOCKED);

                    dirs_[native_socket] = { client };
                    select_.SetEventMask(socket, EV_IO);

                    async_.CallEvents(client, IAsyncCall::CONNECT, nullptr);

                    return dirs_.size();

                }
            }

            return DEFAULT_E_CONNECT;

        }

        socket_result_t ServerHelper::destroy_direction(const SocketPtr& socket)
        {

            select_.SetEventMask(socket);

            const auto& it = dirs_.find(*socket);

            if (connection != nullptr)
            {
                async_.CallEvents(it->second.client, IAsyncCall::DISCONNECT, (void*)socket.get());
                connection->Reject(it->second.client);
            }

            dirs_.erase(it);

        }

        socket_result_t ServerHelper::process_direction(const SocketPtr& socket, event_mask_t eventMask)
        {

            socket_result_t rc = DEFAULT_E_INVARG;

            const auto& it = dirs_.find(*socket);

            // если данное направление существует, то обрабатываем его

            if (it != dirs_.end())
            {
                IClient& client = *it->second.client;

                const SocketPtr& socket = client.GetHandle();

                if ((eventMask & EV_READ) != EV_NONE)
                {
                    // TODO: решение временное (костыль)
                    unsigned char r_buff[1600];
                    // пришли данные на чтение
                    auto r_rc = client.Recv(r_buff, sizeof(r_buff));

                    // диспетчеризация по коду ошибки
                    if (r_rc < 0)
                    {
                        // не обязательно ошибка

                        if (r_rc == DEFAULT_E_UNBLOCK)
                        {
                            // операция не блокирующая
                            // ничего не делаем, в следующем select попадем куда нужно
                        }
                        else
                        {
                            // закрываем направление
                            destroy_direction(socket);
                            return rc = r_rc;
                        }
                    }

                    // такая ситуация происходит когда клиент отсоединился

                    else if (r_rc == 0)
                    {

                        destroy_direction(socket);

                        // закрываем направление

                        return rc = r_rc;
                    }

                    // пришли данные

                    else
                    {
                        async_.CallRead(&client, r_buff, rc, nullptr);
                    }

                }

                // обработка записи
                if ((eventMask & EV_WRITE) != EV_NONE)
                {
                    // выходной буффер пуст, можно отправлять данные

                    unsigned char w_buff[1600];

                    auto w_rc = async_.CallWrite(&client, w_buff, sizeof(w_buff), nullptr);

                    if (w_rc > 0)
                    {
                        // для неблокирующего режима вернет E_UNBLOCK
                        // TODO:  проверку сделаем позже
                        client.Send(w_buff, w_rc);
                    }
                    else
                    {
                        select_.SetEventMask(socket, select_.GetEventMask(socket) & (~EV_WRITE));
                    }

                    //async_.AsyncEventsRead(it->second.client, rd_buff, rc);
                }

                //it->second.
            }

            return rc;
        }
    }
}
