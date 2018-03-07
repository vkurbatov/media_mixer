#include <iostream>

#include "OsnovaLab/Thread/StdTaskPool.h"
#include "OsnovaLab/Functional/StdCaller.h"
#include "OsnovaLab/Thread/StdThread.h"
//#include "OsnovaLab/Builder.h"

#include "OsnovaLab/Thread/StdMutex.h"

OsnovaLab::Thread::StdMutex g_mutex;

#include <chrono>
#include <thread>

void thread_proc(int thread_id)
{
    g_mutex.Lock();

    std::cout << "thread #" << thread_id << " started" << std::endl;
    std::cout << "thread #" << "process..." << std::endl;
    g_mutex.Unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    g_mutex.Lock();
    std::cout << "thread #" << thread_id << " stopped" << std::endl;

    g_mutex.Unlock();
}

void test1()
{

    OsnovaLab::Functional::StdCaller<> caller(thread_proc, 1);

    OsnovaLab::Functional::ICaller<>* icall = &caller;

    //(*icall)();

    //OsnovaLab::Thread::StdThread thr(*icall);

    //thr.Join();

    const int THREAD_COUNT = 5;


    OsnovaLab::Thread::StdTaskPool pool(THREAD_COUNT);
    OsnovaLab::Functional::ICaller<>* callers[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        callers[i] = new OsnovaLab::Functional::StdCaller<>(thread_proc, i);
        pool.GetTask()->PushTask(callers[i]);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    for (auto c : callers)
    {
        delete c;
    }

}

#include <algorithm>

#include "OsnovaLab/Sockets/IP4/UdpServerChannel.h"
#include "OsnovaLab/Sockets/IP4/IP4ConnectionString.h"
#include "OsnovaLab/Sockets/Select.h"
#include "OsnovaLab/Builder.h"

#include "OsnovaLab/Sockets/RawChannel.h"
#include "OsnovaLab/Sockets/RawConnectionString.h"

OsnovaLab::Sockets::IP4::IP4ConnectionString g_serverString("127.0.0.1:5001");
OsnovaLab::Sockets::IP4::IP4ConnectionString g_clientString("127.0.0.1:5000");

#pragma pack(push,1)

struct IP4
{
unsigned char  len:4;			// длина заголовка
unsigned char  ver : 4;			// версия заголовка
unsigned char  tos;				// тип сервиса
unsigned short length;			// длина всего пакета
unsigned short id;				// Id
unsigned short flgs_offset;		// флаги и смещение
unsigned char  ttl;				// время жизни
unsigned char  protocol;		// протокол
unsigned short xsum;			// контрольная сумма
unsigned long  src;				// IP-адрес отправителя
unsigned long  dest;			// IP-адрес назначения
//unsigned short params[4];		// параметры (до 320 бит)
//unsigned char  data[0];		// данные (до 65535 октетов)
};

struct UDP
{
    unsigned short port_src;
    unsigned short port_dst;
    unsigned short length;
    unsigned short crc;
    char data[50];
};

struct UDPIP4
{
    IP4	ip4_header;
    UDP udp_header;
};

#pragma pack(pop)


void thread_simple_udp(int thread_id)
{
    int fd = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in sin = { 0 };

    socklen_t len = sizeof(sin);

    sockaddr sa;

    sa.sa_family = PF_INET;

    //::bind(fd, &sa, sizeof(sa));

    int broadcast = 1;

    int rc =  setsockopt(fd, SOL_SOCKET, SO_BROADCAST,
                &broadcast, sizeof broadcast);

    char buff[1024];

    rc = ::recvfrom(fd, buff, sizeof(buff), 0, (sockaddr*)&sin, &len);

    rc = rc;

    close(fd);


}

void thread_server_raw(int thread_id)
{
    auto log = OsnovaLab::Builder::CreateScreenLogger("[server #" + std::to_string(thread_id) + "] ");

    log->PrintLn("stared");
    OsnovaLab::Sockets::RawConnectionString connectionString;
    OsnovaLab::Sockets::RawChannel raw;
    OsnovaLab::Sockets::IChannel& rawChannel = raw;
    OsnovaLab::Sockets::IConnection* rawConnection = rawChannel.Open(connectionString);
    OsnovaLab::Sockets::IClient* rawIO = rawConnection->Accept(connectionString);

    //OsnovaLab::Sockets::Windows::IOCP iocp;

    OsnovaLab::Sockets::Mode(*rawIO->GetHandle(), OsnovaLab::Sockets::SOCKET_UNBLOCKED);

    OsnovaLab::Sockets::Select select;

    unsigned char buff[1024];
    char hex[(sizeof(buff) + 1) *3];
    int rc = 0;

    //auto sel = ::select(1, &read, NULL, NULL, NULL);

    select.SetEventMask(rawIO->GetHandle(), OsnovaLab::Sockets::EV_IO);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    while (select.Wait() >= 0)
    {
        while ((rc = rawIO->Recv(buff, sizeof(buff))) > 0)
        {
            IP4* ip = (IP4*)buff;

            //UDP* udp = (UDP*)(&ip->params[0] + ip->len);

            UDPIP4* udpip = (UDPIP4*)buff;
            UDP* udp0 = (UDP*)(((char*)ip) + 20);
            UDP* udp = (UDP*)(buff + 20);

            //int off = (int)ip->data - (int)udp;

            log->PrintLn("Recieve %d bytes: %s", htons(udp->length), udp->data);


            /*
            char *p = hex;
            for (int i = 0; i < rc; i++)
            {
                sprintf(p, "%02X ", buff[i]);
                p += 3;
            }

            *p = 0;
            log->PrintLn("Recieve %d bytes: \n%s", rc, hex);
            */
        }
    }


    log->PrintLn("stopped");
}


void thread_server(int thread_id)
{
    auto log = OsnovaLab::Builder::CreateScreenLogger("[server #" + std::to_string(thread_id) + "] ");

    log->PrintLn("stared");

    OsnovaLab::Sockets::IP4::UdpServerChannel server;
    OsnovaLab::Sockets::IChannel& srvChannel = server;
    OsnovaLab::Sockets::IConnection* srvConnection = srvChannel.Open(g_serverString);
    OsnovaLab::Sockets::IClient* srvIO = srvConnection->Accept(g_clientString);

    //OsnovaLab::Sockets::Windows::IOCP iocp;



    OsnovaLab::Sockets::Mode(*srvIO->GetHandle(), OsnovaLab::Sockets::SOCKET_UNBLOCKED);

    OsnovaLab::Sockets::Select select;

    char buff[1024];
    int rc = 0;

    select.SetEventMask(srvIO->GetHandle(), OsnovaLab::Sockets::EV_IO);

    while (select.Wait() >= 0)
    {
        if ((rc = srvIO->Recv(buff, sizeof(buff))) > 0)
        {
            log->PrintLn("Recieve %d bytes: %s", rc, buff);
            if (buff[0] == '\0')
                break;
            std::string str(buff);
            std::reverse(str.begin(), str.end());
            rc = srvIO->Send(const_cast<char*>(str.c_str()), str.length() + 1);
            if (rc < 0)
            {
                rc = rc;
            }
            select.Wait();
            log->PrintLn("Sending %d bytes: %s", rc, str.c_str());
        }
    }


    log->PrintLn("stopped");
}


void thread_client(int thread_id)
{
    auto log = OsnovaLab::Builder::CreateScreenLogger("[client #" + std::to_string(thread_id) + "] ");
    log->PrintLn("stared");
    std::string a_strings[] = { "Kurbatov", "Vasiliy", "Vladimirovich", ""};


    OsnovaLab::Sockets::IP4::UdpServerChannel client;
    OsnovaLab::Sockets::IChannel& cliChannel = client;
    OsnovaLab::Sockets::IConnection* cliConnection = cliChannel.Open(g_clientString);
    OsnovaLab::Sockets::IClient* cliIO = cliConnection->Accept(g_serverString);

    char buff[1024];
    size_t rc = 0;

    for (auto& s : a_strings)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if ((rc = cliIO->Send(const_cast<char*>(s.c_str()), s.length() + 1)) > 0)
        {
            log->PrintLn("Sending %d bytes: %s", rc, s.c_str());
            /*if ((rc = cliIO->Recv(buff, sizeof(buff))) > 0)
            {
                log->PrintLn("Recieve %d bytes: %s", rc, buff);
            }*/
        }
    }

    log->PrintLn("stopped");
}

void test2()
{
    OsnovaLab::Thread::StdTaskPool pool(2);
    pool.GetTask()->PushTask(new OsnovaLab::Functional::StdCaller<>(thread_server_raw, 1));
    pool.GetTask()->PushTask(new OsnovaLab::Functional::StdCaller<>(thread_client, 2));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //std::cout << "rtp_listener" << std::endl;
}

#include "OsnovaLab/Sockets/IP4/TcpServerChannel.h"
#include "OsnovaLab/Sockets/IP4/TcpClientChannel.h"

void tcp_server_thread(int thread_id)
{
    auto log = OsnovaLab::Builder::CreateScreenLogger("[TCP server #" + std::to_string(thread_id) + "] ");

    log->PrintLn("stared");

    OsnovaLab::Sockets::IP4::TcpServerChannel srvChnl;
    auto srvConn = srvChnl.Open(g_serverString);
    auto srvCli = srvConn->Accept(g_serverString);

    OsnovaLab::Sockets::socket_size_t rc = 0;

    char recv[1024] = {0};

    while((rc = srvCli->Recv(recv, sizeof(recv))) > 0)
    {
        log->PrintLn("Recive %d bytes: %s", rc, recv);

        std::string str(recv);
        std::reverse(str.begin(),str.end());

        if ((rc = srvCli->Send(const_cast<char*>(str.c_str()), str.length() + 1)) >= 0)
        {
            log->PrintLn("Sending %d bytes: %s", rc, str.c_str());
        }

    }

    if (rc == 0)
        log->PrintLn("Client %s disconnect", g_serverString.GetConnectionString().c_str());

    log->PrintLn("stopped");
}

void tcp_client_thread(int thread_id)
{
    auto log = OsnovaLab::Builder::CreateScreenLogger("[TCP client #" + std::to_string(thread_id) + "] ");

    log->PrintLn("stared");

    OsnovaLab::Sockets::IP4::TcpClientChannel cliChnl;
    auto cliConn = cliChnl.Open(g_serverString);
    auto cliCli = cliConn->Accept(g_serverString);

    OsnovaLab::Sockets::socket_size_t rc = 0;

    std::string str("Hello, World!");

    char recv[1024];

    if ((rc = cliCli->Send(const_cast<char*>(str.c_str()), str.length() + 1)) >= 0)
    {

        log->PrintLn("Sending %d bytes: %s", rc, str.c_str());

        if ((rc = cliCli->Recv(recv, sizeof(recv))) >= 0)
        {
            log->PrintLn("Revice %d bytes: %s", rc, recv);
        }

    }



    log->PrintLn("stopped");
}

void test3()
{
    OsnovaLab::Thread::StdTaskPool pool(2);
    pool.GetTask()->PushTask(new OsnovaLab::Functional::StdCaller<>(tcp_server_thread, 1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pool.GetTask()->PushTask(new OsnovaLab::Functional::StdCaller<>(tcp_client_thread, 2));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //std::cout << "rtp_listener" << std::endl;
    //auto log = OsnovaLab::Builder::CreateScreenLogger("main ");
    //log->PrintLn("Test log");
    test3();
    //std::cout << "Hello World!" << std::endl;
    //log->PrintLn("Test log");
    std::cout << "rtp_listener" << std::endl;
    std::cin.get();
    return 0;
}

