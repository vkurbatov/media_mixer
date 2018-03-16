#include "net/net.h"

#include <sys/types.h>
#include <errno.h>
#include <unistd.h>         // close

namespace mmx
{

    namespace net
    {
        int	close_sock(int sock)
        {
            int rc = -EINVAL;

            shutdown(sock, SHUT_RDWR);
            rc = ::close(sock);

            if (rc < 0)
            {
                rc = -errno;
            }

            return rc;
        }
    }
}
