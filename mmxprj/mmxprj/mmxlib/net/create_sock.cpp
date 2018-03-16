#include "net/net.h"

#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>


namespace mmx
{
    namespace net
    {
        // создает сокет оперделенного типа, а также опционально
        // связывает, коннектит и переводит в слушающий режим

        int	create_sock(int type,
            int ipproto,
            socket_addr_t local_addr,
            socklen_t local_size,
            socket_addr_t remote_addr,
            socklen_t remote_size,
            int flags,
            int backlog)
        {

            int rc = -EINVAL;

            // создаем сокет по параметрам

            int sock = socket(AF_INET, type, ipproto);

            // сокет создан

            if (sock >= 0)
            {
                int state = 1;

                rc = 0;

                while (state != 0)
                {
                    switch(state)
                    {

                    case 1:         // получение флагов

                        if (flags != 0)
                        {
                            rc = fcntl(sock, F_GETFL, 0);

                            if (rc >= 0)
                            {
                                // флаги получены, соеденим их с маской

                                flags |= rc;

                                // и на установку

                                state = 2;
                            }
                        }
                        else
                        {
                            // флаги не заданы, сразу на связывание

                            state = 3;
                        }

                        break;

                    case 2:         // запись флагов

                        rc = fcntl(sock, F_SETFL, flags);

                        if (rc >= 0)
                        {

                            // флаги установлены, на связывание

                            state = 3;
                        }

                        break;

                    case 3:         // связывание с локальным адресом

                        if (local_addr != nullptr && local_size > 0)
                        {
                            rc = bind(sock, (sockaddr*)(local_addr), local_size);
                        }

                        if (rc >= 0)
                        {
                            state = 4;
                        }

                        break;

                    case 4:         // связывание с удаленным адресом

                        if (remote_addr != nullptr && remote_size > 0)
                        {
                            // сокет не может быть одновременно соединенным и слушающим

                            if (backlog > 0)
                            {
                                rc =-EINVAL;
                            }
                            else
                            {
                                rc = connect(sock, (sockaddr*)(remote_addr), remote_size);

                                // connect может вернуть EWOULDBLOCK если установлен
                                // неблокирующий режим. Это нормальная ситуация

                                if (rc >= 0 || errno == EWOULDBLOCK)
                                {
                                    state = 0;
                                }

                            }
                        }
                        else
                        {
                            state = 5;
                        }
                        break;

                    case 5:         // установка слушающего сокета
                        if (backlog > 0)
                        {
                            rc = listen(sock, backlog);

                            if (rc >= 0)
                            {
                                state = 0;
                            }

                        }
                        else
                            state = 0;
                        break;

                    case -1:        // ошибка

                        // возвращаем ошибку

                        rc = -errno;

                        close_sock(sock);

                        state = 0;

                        break;

                    }// switch(state...

                    if (rc < 0)
                    {
                        state = -1;
                    }

                }//while(state...
                if (rc >= 0)
                {
                    rc = sock;
                }

            }

            return rc;
        }
    }
}
