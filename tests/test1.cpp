#include <chrono>
#include <algorithm>
#include <thread>

#include "tests/test.h"

#include "OsnovaLab/Builder.h"
#include "OsnovaLab/Sockets/ServerDispatcher.h"
#include "OsnovaLab/Thread/StdTaskPool.h"
#include "OsnovaLab/Functional/StdCaller.h"

#include "OsnovaLab/Sockets/Select.h"
#include "OsnovaLab/Sockets/IP4/TcpServerChannel.h"
#include "OsnovaLab/Sockets/IP4/TcpClientChannel.h"
#include "OsnovaLab/Sockets/IP4/IP4ConnectionString.h"

namespace OsnovaLab
{
    namespace Tests
    {

        class Asio : public Sockets::IAsyncCall
        {
            Logger::ILogger& logger_;

        public:
            Asio(Logger::ILogger& logger) :
                logger_(logger)
            {

            }

            Sockets::socket_size_t CallRead(Sockets::IClient* client, Sockets::packet_data_t data, Sockets::socket_size_t size, void* context) override
            {
                std::string str((const char*)data);
                logger_.PrintLn("From sock=%d recieve %d bytes: %s ", (Sockets::native_socket_t)*client->GetHandle().get(), size, data);

                std::reverse(str.begin(), str.end());

                auto rc = client->Send(const_cast<char*>(str.data()), str.length() + 1);

                logger_.PrintLn("From sock=%d send %d bytes: %s ", (Sockets::native_socket_t)*client->GetHandle().get(), rc, str.data());

            }

            Sockets::socket_size_t CallWrite(Sockets::IClient* client, Sockets::packet_data_t data, Sockets::socket_size_t size, void* context) override
            {
                return 0;
            }

            Sockets::socket_size_t CallEvents(Sockets::IClient* client, const AsyncEvents& event, void* context) override
            {

                switch(event)
                {
                    case AsyncEvents::CONNECT:
                        logger_.PrintLn("Socket %d connected!", (Sockets::native_socket_t)*client->GetHandle().get());
                    break;
                    case AsyncEvents::DISCONNECT:
                        logger_.PrintLn("Socket %d disconnected!", (Sockets::native_socket_t)*client->GetHandle().get());
                    break;
                }
            }

        };

        void server_thread(int thread_id)
        {
            auto log = Builder::CreateScreenLogger("[TCP server #" + std::to_string(thread_id) + "] ");
            log->PrintLn("Start!");

            OsnovaLab::Sockets::IP4::IP4ConnectionString serverString("127.0.0.1:5001");
            OsnovaLab::Sockets::IP4::TcpServerChannel channel;
            OsnovaLab::Sockets::Select select;
            Asio asio(*log);
            OsnovaLab::Sockets::ServerDispatcher dispatcher(channel, serverString, asio, select);

            int i = 0;
            while (1)
            {
                auto d_rc = dispatcher.Dispatch();
                log->PrintLn("Dispatching #%d witch result = %d", i++, d_rc);
            }

            log->PrintLn("Stop!");
        }

        void client_thread(int thread_id)
        {
            auto log = Builder::CreateScreenLogger("[TCP client #" + std::to_string(thread_id) + "] ");
            log->PrintLn("Start!");

            OsnovaLab::Sockets::IP4::IP4ConnectionString serverString("127.0.0.1:5001");
            OsnovaLab::Sockets::IP4::TcpClientChannel channel;

            auto conn = channel.Open(serverString);

            auto cli = conn->Accept(serverString);

            char buff[] = "Hello, world!";
            char recv[sizeof(buff)];

            auto rc = cli->Send(buff, sizeof(buff));

            log->PrintLn("Sending %d bytes to server", rc);

            rc = cli->Recv(recv,sizeof(recv));

            log->PrintLn("Recieve %d bytes from server", rc);

            log->PrintLn("Stop!");
        }

        int test1()
        {
            Thread::StdTaskPool pool(10);
            Functional::StdCaller<> server_call(server_thread, 1);

            Functional::StdCaller<> client_call(client_thread, 2);

            pool.GetTask()->PushTask(&server_call);


            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            int i = 9;
            while (--i > 0)
                pool.GetTask()->PushTask(&client_call);

            std::this_thread::sleep_for(std::chrono::milliseconds(10000));

            return 0;
        }
    }
}
