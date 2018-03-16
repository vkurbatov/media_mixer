#include "defines.h"

#include "mmxlib/logs/log.h"
#include "mmxlib/net/net.h"
#include "mmxlib/headers/udp.h"

#include <netdb.h>      // IPPROTO_UDP
#include <fcntl.h>      // O_NONBLOCK
#include <errno.h>
#include <cstdio>       // sprintf

void packet_printer(unsigned short port, char* data, int size)
{
    char buff[1024] = {0};

    char *pstr = buff;

    int s = size;

    while (s-- > 0)
    {
        pstr += std::sprintf(pstr, "%02x, ", *data++);
    }

    mmx::logs::logD("packet_printer: print %d bytes on port = %d: %s", size, port, buff);
}

int listener(unsigned int addr, unsigned short begin, unsigned short end, int step)
{

    int rc = -EINVAL;

    // для подсчета вызовов

    static int s_listener_id = 0;


    // для raw сокетов нужно задать адрес с доменом

    sockaddr sa_local;
    sa_local.sa_family = AF_INET;

    int listener_id = ++s_listener_id;

    // будем только слушать сокет

    fd_set rd_set;

    FD_ZERO(&rd_set);

    mmx::logs::logD("[listener(%d)] (addr=%d, port=%d, count=%d, step=%d) started!", listener_id, addr, begin, end, step);

    // сперва попытаемся создать направление для обработки данных

    int dir_id = create_dir(begin, end, step);

    if (dir_id >= 0)
    {
        int sock = mmx::net::create_sock(SOCK_RAW, IPPROTO_UDP, &sa_local, sizeof(sa_local), nullptr, 0, O_NONBLOCK);

        if (sock >= 0)
        {
            mmx::logs::logI("[listener(%d)] socket %d create success", listener_id, sock);

            bool run = true;

            while (run)
            {

                FD_SET(sock, &rd_set);

                rc = ::select(sock+1, &rd_set, nullptr, nullptr, nullptr);

                if (rc > 0)
                {
                    if (FD_ISSET(sock, &rd_set))
                    {
                        if ((rc = dispatch(sock, dir_id)) >= 0)
                        {
                            // тестовая задача

                            process_dir(dir_id, packet_printer);
                        }
                    }
                    else
                    {
                        // select сработал, а единственный дескриптор не установлен...
                    }
                }
                else
                {
                    switch(errno)
                    {
                        case EWOULDBLOCK: // это нормальная ситуация, правда не понятно почему она сработала в select


                            break;
                    }

                    mmx::logs::logE("[listener(%d)] select error rc = %d", listener_id, rc);

                }
            }
        }
        else
        {
            mmx::logs::logE("[listener(%d)] error=%d for create socket", listener_id, sock);
        }//sock

        destroy_dir(dir_id);

    } //dir
    else
    {
         mmx::logs::logE("[listener(%d)] error create direction rc = %d", listener_id, rc = dir_id);
    }

    mmx::logs::logD("[listener(%d)] stop with rc = %d", listener_id, rc);

    return rc;
}
