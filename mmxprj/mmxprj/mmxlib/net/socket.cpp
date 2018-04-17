#include "socket.h"

#include <cstring>

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>	// sockaddr_in
#include <unistd.h>     // close
#include <fcntl.h>      // fcntl
#include <arpa/inet.h>  // inet_ntoa....

namespace mmx
{
    namespace net
    {
    /*
        Socket::Socket() :
            type_(-1),
            proto_(-1),
            handle_(-1),
            l_address_(0),l_port_(0),
            r_address_(0),r_port_(0),
            backlog_(0)
        {

        }
    */
        Socket::Socket(int type, int proto) :
            type_(type),
            proto_(proto),
            handle_(-1),
            l_address_(0),l_port_(0),
            r_address_(0),r_port_(0),
            backlog_(0)
        {

        }
/*
        Socket::Socket(const Socket& socket, int flags) :
            type_(socket.type_),
            proto_(socket.proto_),
            handle_(-1),
            l_address_(socket.l_address_),l_port_(socket.l_port_),
            r_address_(0),r_port_(0),
            backlog_(0)
        {
            // если шаблонный сокет - открытый серверный, то значит пытаемся
            // выполнить accept

            if (socket.backlog_ > 0 && socket.handle_>= 0)
            {

                handle_ = Accept(socket.handle_, &r_address_, &r_port_);

                if (handle_ > 0)
                {
                    if (flags != 0)
                    {
                        SetFlags(handle_, flags);
                    }
                }

            }
        }
*/
        Socket::Socket(Socket&& socket) :
            type_(socket.type_),
            proto_(socket.proto_),
            handle_(socket.handle_),
            l_address_(socket.l_address_),l_port_(socket.l_port_),
            r_address_(socket.r_address_),r_port_(socket.r_port_),
            backlog_(socket.backlog_)
        {
            socket.type_ = -1;
            socket.proto_ = -1;
            socket.handle_ = -1;
            socket.l_address_ = 0;
            socket.l_port_ = 0;
            socket.r_address_ = 0;
            socket.r_port_ = 0;
            socket.backlog_ = 0;
        }

        Socket::~Socket()
        {
            if (handle_ >= 0)
            {
                Close();
            }
        }
        int Socket::Create(int type, int proto)
        {

            int rc = -EINVAL;

            if (type >= 0 && proto >= 0)
            {

                rc = ::socket(PF_INET, type, proto);

                if (rc < 0)
                {
                    rc = -errno;
                }
            }

            return rc;

        }

        int Socket::Close(int sock)
        {

            int rc = -EINVAL;

            if (sock >= 0)
            {

                rc = ::close(sock);

                if (rc < 0)
                {
                    rc = -errno;
                }
            }

            return rc;

        }


        int Socket::Send(int sock, const void* data, int size, int flags, address_t r_address, port_t r_port)
        {
            int rc = -EINVAL;

            if (sock >= 0 && data != nullptr && size > 0)
            {
                // если указан адрес, то вызов sendto

                if (r_address != 0)
                {
                    sockaddr_in sin = {0};
                    sin.sin_family = PF_INET;
                    sin.sin_addr.s_addr = ::htonl(r_address);
                    sin.sin_port = ::htons(r_port);
                    rc = ::sendto(sock, data, size, flags, (sockaddr*)&sin, sizeof(sin));
                }
                else
                {
                    rc = ::send(sock, data, size, flags);
                }

                if (rc < 0)
                {
                    rc = -errno;
                }

            }

            return rc;
        }

        int Socket::Recv(int sock, void* data, int size, int flags, address_t* r_address, port_t* r_port)
        {

            int rc = -EINVAL;

            if (sock >= 0 && data != nullptr && size > 0)
            {
                // если передан указатель адреса или порта, то вызов recvfrom

                if (r_address != nullptr || r_port != nullptr)
                {
                    sockaddr_in sin = {0};
                    socklen_t sin_len = sizeof(sockaddr_in);
                    sin.sin_family = PF_INET;
                    sin.sin_addr.s_addr = r_address == nullptr ? 0 : ::htonl(*r_address);
                    sin.sin_port = r_port == nullptr ? 0 : ::htons(*r_port);

                    rc = ::recvfrom(sock, data, size, flags, (sockaddr*)&sin, &sin_len);

                    if (r_address != nullptr)
                    {
                        *r_address = ::ntohl(sin.sin_addr.s_addr);
                    }

                    if (r_port != nullptr)
                    {
                        *r_port = ::ntohs(sin.sin_port);
                    }

                }
                else
                {
                    rc = ::recv(sock, data, size, flags);
                }

                if (rc < 0)
                {
                    rc = -errno;
                }

            }

            return rc;
        }

        int Socket::Bind(int sock, address_t l_address, port_t l_port)
        {

            int rc = -EINVAL;

            if (sock >= 0)
            {
                if (l_address == 0 && l_port == 0)
                {
                    sockaddr sa = { 0 };

                    sa.sa_family = PF_INET;

                    rc = ::bind(sock, &sa, sizeof(sa));
                }
                else
                {
                    sockaddr_in sin = { 0 };

                    sin.sin_family = PF_INET;
                    sin.sin_addr.s_addr = ::htonl(l_address);
                    sin.sin_port = ::htons(l_port);

                    rc = ::bind(sock, (sockaddr*)&sin, sizeof(sin));
                }

                if (rc < 0 )
                {
                    rc = -errno;
                }

            }

            return rc;
        }

        int Socket::Connect(int sock, address_t r_address, port_t r_port)
        {
            int rc = -EINVAL;

            if (sock >= 0)
            {
                if (r_address == 0 && r_port == 0)
                {
                    sockaddr sa = { 0 };

                    sa.sa_family = PF_INET;

                    rc = ::connect(sock, &sa, sizeof(sa));
                }
                else
                {
                    sockaddr_in sin = { 0 };

                    sin.sin_family = PF_INET;
                    sin.sin_addr.s_addr = ::htonl(r_address);
                    sin.sin_port = ::htons(r_port);

                    rc = ::connect(sock, (sockaddr*)&sin, sizeof(sin));
                }

                if (rc < 0 )
                {
                    rc = -errno;
                }

            }

            return rc;
        }

        int Socket::Accept(int sock, address_t* r_address, port_t* r_port)
        {
            int rc = -EINVAL;

            if (sock >= 0)
            {

                sockaddr_in sin = { 0 };

                sin.sin_family = PF_INET;
                socklen_t len = sizeof(sin);

                rc = ::accept(sock, (sockaddr*)&sin, &len);

                if (rc < 0 )
                {
                    rc = -errno;
                }
                else
                {
                    if (r_address != nullptr)
                    {
                        *r_address = ::ntohl(sin.sin_addr.s_addr);
                    }

                    if (r_port != nullptr)
                    {
                        *r_port = ::ntohs(sin.sin_port);
                    }
                }

            }

            return rc;
        }

        int Socket::Listen(int sock, int backlog)
        {
            int rc = -EINVAL;

            if (sock >= 0)
            {
                rc = ::listen(sock, backlog);

                if (rc < 0)
                {
                    rc = -errno;
                }
            }

            return rc;
        }

        int Socket::SetOption(int sock, int level, int option, const void* value, int size)
        {
            int rc = -EINVAL;

            if (sock >= 0 && size>0 && value != nullptr)
            {
                rc = ::setsockopt(sock, level, option, value, size);

                if (rc < 0)
                {
                    rc = -errno;
                }
            }

            return rc;

        }

        int Socket::GetOption(int sock, int level, int option, void* value, int size)
        {
            int rc = -EINVAL;

            if (sock >= 0 && size>0 && value != nullptr)
            {
                socklen_t len = size;

                rc = ::getsockopt(sock, level, option, value, &len);

                if (rc < 0)
                {
                    rc -= errno;
                }
                else
                {
                    rc = len;
                }

            }

            return rc;
        }

        int Socket::SetFlags(int sock, int flags)
        {
            int rc = -EINVAL;

            if (sock >= 0)
            {
                rc = ::fcntl(sock, F_GETFL, 0);

                if (rc >= 0 && flags != 0)
                {
                    rc = ::fcntl(sock, F_SETFL, flags | rc);
                }

                if (rc < 0)
                {
                    rc = -errno;
                }
            }

            return rc;
        }

        int  Socket::ClrFlags(int sock, int flags)
        {
            int rc = -EINVAL;

            if (sock >= 0)
            {
                rc = ::fcntl(sock, F_GETFL, 0);

                if (rc >= 0 && flags != 0)
                {
                    rc = ::fcntl(sock, F_SETFL, (~flags) & rc);
                }

                if (rc < 0)
                {
                    rc = -errno;
                }
            }

            return rc;
        }

        char* Socket::AtoS(address_t address, char* abuff)
        {
            char* rc = nullptr;

            in_addr ia =  { ::ntohl(address) };

            rc = ::inet_ntoa(ia);

            if (abuff != nullptr)
            {
                rc = std::strcpy(abuff, rc);
            }

            return rc;
        }

        address_t Socket::StoA(const char* addr_str)
        {
            address_t rc = INADDR_NONE;

            if (addr_str != nullptr)
            {

                rc = ::ntohl(::inet_addr(addr_str));

            }

            return rc;
        }

        int Socket::Open(address_t l_address, port_t l_port, address_t r_address, port_t r_port, int backlog, int flags)
        {
            int rc = -EPROTOTYPE;

            if (type_ >= 0 && proto_ >= 0)
            {
                rc = -EBUSY;

                if (handle_ < 0)
                {

                    enum s_state_t : int
                    {
                        S_EXIT = 0,
                        S_INIT = 1,
                        S_GETF = 2,
                        S_SETF = 3,
                        S_BIND = 4,
                        S_CONN = 5,
                        S_LIST = 6,
                        S_ERR = -1
                    };

                    s_state_t state = S_INIT;

                    while (state != S_EXIT)
                    {
                        switch (state)
                        {

                        case S_INIT:

                            rc = Create(type_, proto_);

                            if (rc >= 0)
                            {
                                handle_ = rc;
                            }

                            state = S_SETF;

                            break;

                        case S_SETF:

                            rc = SetFlags(handle_, flags);

                            state = S_BIND;

                            break;

                        case S_BIND:

                            if (l_port != 0 || type_ == SOCK_RAW)
                            {
                                if (type_ == SOCK_STREAM)
                                {
                                    int en = 1;
                                    SetOption(handle_, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int));
                                    SetOption(handle_, SOL_SOCKET, SO_REUSEPORT, &en, sizeof(int));
                                }

                                rc = Bind(handle_, l_address, l_port);
                            }

                            state = S_CONN;

                            break;

                        case S_CONN:

                            if (r_port != 0 && r_address != 0)
                            {

                                rc = Connect(handle_, r_address, r_port);

                                // после connect нет никакого listen

                                if (rc >= 0)
                                {
                                    state = S_EXIT;
                                }

                            }
                            else
                            {
                                state = S_LIST;
                            }

                            break;

                        case S_LIST:

                            if (backlog > 0)
                            {
                                rc = Listen(handle_, backlog);
                            }

                            if (rc >= 0)
                            {
                                state = S_EXIT;
                            }

                            break;

                        case S_ERR:

                            if (handle_ >= 0)
                            {

                                close(handle_);

                                handle_ = -1;

                                state = S_EXIT;

                            }

                            break;
                        }

                        if (state!= S_EXIT && rc < 0)
                        {

                            if (rc == -EWOULDBLOCK) // после connect
                            {
                                // эта ситуация нормальная, просто выходим

                                state = S_EXIT;
                            }
                            else
                            {
                                state = S_ERR;
                            }
                        }
                    }

                    if (handle_ >= 0)
                    {
                        rc = handle_;

                        l_address_ = l_address;
                        l_port_ = l_port;

                        r_address_ = r_address;
                        r_port_ = r_port;

                        backlog_ = backlog;

                    }
                }
            }

            return rc;
        }

        int Socket::Accept(const Socket& socket, int flags)
        {
            int rc = -EINVAL;

            if (type_ == SOCK_STREAM && socket.type_ == SOCK_STREAM && socket.backlog_ > 0)
            {

                rc = -EBUSY;

                if (handle_ < 0)
                {
                    rc = handle_ = Accept(socket.handle_, &r_address_, &r_port_);

                    if (handle_ >= 0)
                    {
                        l_address_ = socket.l_address_;
                        l_port_ = socket.l_port_;

                        if (flags != 0)
                        {
                            SetFlags(handle_, flags);
                        }
                    }
                }

            }

            return rc;
        }

        int Socket::Close()
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                rc = close(handle_);

                handle_ = -1;

                if (rc < 0)
                {
                    rc = -errno;
                }
            }

            return rc;
        }

        int Socket::Send(const void* data, int size, int flags, address_t r_address, port_t r_port)
        {
            return Send(handle_, data, size, flags, r_address, r_port);
        }

        int Socket::Recv(void* data, int size, int flags, address_t* r_address, port_t* r_port)
        {
            return Recv(handle_, data, size, flags, r_address, r_port);
        }

        int Socket::SetOption(int level, int option, long value)
        {
            return SetOption(handle_, level, option, &value, sizeof(long));
        }

        int Socket::GetOption(int level, int option, long* value)
        {
            return GetOption(handle_, level, option, value, sizeof(long));
        }

        int Socket::SetOption(int level, int option, int value)
        {
            return SetOption(handle_, level, option, &value, sizeof(int));
        }

        int Socket::GetOption(int level, int option, int* value)
        {
            return GetOption(handle_, level, option, value, sizeof(int));
        }

        int Socket::ClrFlags(int flags)
        {
            return ClrFlags(handle_, flags);
        }

        int Socket::SetFlags(int flags)
        {
            return SetFlags(handle_, flags);
        }

        address_t Socket::LocalAddress() const
        {
            return l_address_;
        }

        port_t Socket::LocalPort() const
        {
            return l_port_;
        }

        address_t Socket::RemoteAddress() const
        {
            return r_address_;
        }

        port_t Socket::RemotePort() const
        {
            return r_port_;
        }


        int Socket::Backlog() const
        {
            return backlog_;
        }

        int Socket::Handle() const
        {
            return handle_;
        }

        Socket::operator int() const
        {
            return handle_;
        }

        int Socket::Type() const
        {
            return type_;
        }

        int Socket::Proto() const
        {
            return proto_;
        }

    }
}
