#include "net/net.h"

#include <sys/types.h>
#include <errno.h>

namespace mmx
{

    namespace net
    {
        int	read_sock(int sock, socket_data_t data, int size, int flags,
            socket_addr_t addr, socklen_t addr_len)
        {
            int rc = -EINVAL;

            if (addr == nullptr)
            {
                rc = recv(sock, (char*) data, size, flags);
            }
            else

            {
                if (addr_len > 0)
                {
                    rc = recvfrom(sock, (char*) data, size, flags, (sockaddr*)addr, &addr_len);
                }
            }

            if (rc == -1)
            {
                rc = -errno;
            }

            return rc;
        }
    }
}
