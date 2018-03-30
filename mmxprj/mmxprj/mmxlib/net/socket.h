#ifndef _MMX_NET_SOCKET_H
#define _MMX_NET_SOCKET_H

//#include <sys/socket.h>

namespace mmx
{
    namespace net
    {
        using address_t = unsigned int;
        using port_t = unsigned short;

        class Socket
        {
            int handle_;
            int type_;
            int proto_;
            address_t l_address_;
            port_t l_port_;
            address_t r_address_;
            port_t r_port_;
            int backlog_;

        public:

            // конструкторы/деструкторы

            //Socket();
            Socket(Socket&& socket);
            Socket(const Socket& socket, int flags = 0);
            Socket(int type, int proto = 0);
            ~Socket();

            // статические методы для работы с сокетными дескрипторами

            static int Create(int type, int proto = 0);
            static int Close(int sock);
            static int Send(int sock, const char* data, int size, int flags = 0, address_t r_address = 0, port_t r_port = 0);
            static int Recv(int sock, char* data, int size, int flags = 0, address_t* r_address = nullptr, port_t* r_port = nullptr);
            static int Bind(int sock, address_t l_address, port_t l_port);
            static int Connect(int sock, address_t r_address, port_t r_port);
            static int Accept(int sock, address_t* r_address, port_t* r_port);
            static int Listen(int sock, int backlog);
            static int SetOption(int sock, int level, int option, const void* value, int size);
            static int GetOption(int sock, int level, int option, void* value, int size);
            static int SetFlags(int sock, int flags);
            static int ClrFlags(int sock, int flags);
            static char* AtoS(address_t address, char* abuff = nullptr);
            static address_t StoA(const char* addr_str);

            // открыть/закрыть

            int Open(address_t l_address = 0, port_t l_port = 0, address_t r_address = 0, port_t r_port = 0, int backlog = 0, int flags = 0);
            int Close();

            // методы ввода-вывода

            int Send(const char* data, int size, int flags = 0, address_t r_address = 0, port_t r_port = 0);
            int Recv(char* data, int size, int flags = 0, address_t* r_address = nullptr, port_t* r_port = nullptr);

            // мотоды манипуляции c аттрибутами

            int SetOption(int level, int option, long value);
            int GetOption(int level, int option, long* value);
            int SetOption(int level, int option, int value);
            int GetOption(int level, int option, int* value);
            int SetFlags(int flags);
            int ClrFlags(int flags);

            // методы доступа к членам

            address_t LocalAddress() const;
            port_t LocalPort() const;
            address_t RemoteAddress() const;
            port_t RemotePort() const;
            int Backlog() const;
            int Handle() const;
            operator int() const;
            int Type() const;
            int Proto() const;

        };
    }
}

#endif
