#ifndef _MMX_NET_H
#define _MMX_NET_H

#include <sys/socket.h>

namespace mmx
{
	namespace net
	{
		// создает сокет и делает bind и connect

        typedef void* socket_data_t;
        typedef void* socket_addr_t;

		
		int	create_sock(int type, 
			int ipproto = 0, 
            socket_addr_t local_addr = nullptr,
            socklen_t local_size = 0,
            socket_addr_t remote_addr = nullptr,
            socklen_t remote_size = 0,
            int flags = 0,
            int backlog = 0);
			
        int	write_sock(int sock, socket_data_t data, int size, int flags = 0,
            socket_addr_t addr = nullptr, socklen_t addr_len = 0);
			
        int	read_sock(int sock, socket_data_t data, int size, int flags = 0,
            socket_addr_t addr = nullptr, socklen_t addr_len = 0);

        int close_sock(int sock);


        int set_sock_opt(int sock, int level, int option, long value);
        int get_sock_opt(int sock, int level, int option, long* pvalue);
	}
}

#endif
