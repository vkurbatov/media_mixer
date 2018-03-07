#include <chrono>
#include <algorithm>

#include "tests/test.h"

#include "OsnovaLab/Builder.h"
#include "OsnovaLab/Sockets/ServerDispatcher.h"
#include "OsnovaLab/Thread/StdTaskPool.h"
#include "OsnovaLab/Functional/StdCaller.h"

namespace OsnovaLab
{
    namespace Tests
    {

        class Asio : public Sockets::IAsyncCall
        {
            Logger::ILogger* logger_;

        public:
            Asio(const std::string& name) :
                logger_(Builder::CreateScreenLogger(name))
            {

            }

            Sockets::socket_size_t CallRead(Sockets::IClient* client, Sockets::packet_data_t data, Sockets::socket_size_t size, void* context) override
            {
                std::string str((const char*)data);
                logger_->PrintLn("From sock=%d recieve %d bytes: %s ", client->GetHandle().get(), size, data);

                std::reverse(str.begin(), str.end());

                auto rc = client->Send(const_cast<char*>(str.data()), str.length() + 1);

                logger_->PrintLn("From sock=%d send %d bytes: %s ", client->GetHandle().get(), rc, str.data());

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
                        logger_->PrintLn("Socket %d connected!", client->GetHandle().get());
                    break;
                    case AsyncEvents::DISCONNECT:
                        logger_->PrintLn("Socket %d disconnected!", client->GetHandle().get());
                    break;
                }
            }

        };

        void server_thread(int thread_id)
        {
            auto log = Builder::CreateScreenLogger();

        }

        void client_thread(int thread_id)
        {
            auto log = Builder::CreateScreenLogger();

        }

        int test1()
        {
            Thread::StdTaskPool pool;
            Functional::StdCaller<> server_call(server_thread, 1);
            Functional::StdCaller<> client_call(client_thread, 2);


        }
    }
}
