#include "net/net.h"

#include <sys/socket.h>
#include <errno.h>


namespace mmx
{
    namespace net
    {

        int set_sock_opt(int sock, int level, int option, long value)
        {
            int rc = ::setsockopt(sock, level, option, &value, sizeof(value));

            if (rc < 0)
                rc = -errno;

            return rc;
        }

        int get_sock_opt(int sock, int level, int option, long* pvalue)
        {
            socklen_t len = sizeof(long);

            int rc = ::getsockopt(sock, level, option, pvalue, &len);

            if (rc < 0)
                rc = -errno;
            else
                rc = len;

            return rc;
        }
    }
}
