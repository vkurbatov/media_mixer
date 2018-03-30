#include <thread>
#include <chrono>
#include <list>
#include <memory>
#include <vector>
#include <algorithm>

#include "defines.h"

#include "mmxlib/logs/log.h"
#include "mmxlib/net/socket.h"
#include "mmxlib/net/select.h"
#include "mmxlib/ipc/pchannel.h"
#include "mmxlib/headers/media.h"

#include <fcntl.h>
#include <errno.h>
#include <netdb.h>




namespace mmxsrv
{
    int server(const char* channel_name, unsigned int address, unsigned short port)
    {
        int rc = -1;

        mmx::ipc::PipeChannel channel;
        mmx::net::Socket sock(SOCK_STREAM, IPPROTO_TCP);
        mmx::net::Select sel;

        std::list<mmx::net::Socket> clients;

        std::vector<char> buffer(1600);

        // открываем канал в неблокирующем режиме

        enum srv_state_t : int
        {
            S_INIT = 1,
            S_OPEN = 2,
            S_SELECT = 3,
            S_ACCEPT = 4,
            S_READ = 5,
            S_CHECK = 6,
            S_WRITE = 7,
            S_WAIT = 8,
            S_ERR = -1,
            S_EXIT = 0

        } state = S_INIT;

        srv_state_t force_state = S_EXIT;
        mmx::net::timeout_t tout = 0;

        while (state != S_EXIT)
        {

            switch(state)
            {
                case S_INIT:

                    state = S_OPEN;
                    if (channel.Handle() < 0)
                    {
                        rc = channel.Open(channel_name, O_RDONLY | O_NONBLOCK);

                        if (rc < 0)
                        {
                            mmx::logs::logE("(server) channel [%s] open error! (rc = %d)!", channel_name, rc);

                            force_state = S_INIT;
                            tout = 1000;
                            state = S_WAIT;
                        }
                        else
                        {
                            mmx::logs::logI("(server) open channel [%s] success (fd = %d)!", channel_name, rc);
                            sel.Set(rc, mmx::net::S_EV_READ);
                        }
                    }

                    break;

                case S_OPEN:

                    state = S_SELECT;

                    if (sock.Handle() < 0)
                    {
                        rc = sock.Open(address, port, 0, 0, 4, O_NONBLOCK);

                        if (rc < 0)
                        {

                            mmx::logs::logE("(server) socket [%s:%d] open error (rc = %d)!", mmx::net::Socket::AtoS(address), port, rc);

                            force_state = S_OPEN;
                            tout = 1000;
                            state = S_WAIT;

                        }
                        else
                        {
                            mmx::logs::logI("(server) socket [%s:%d] open success (rc = %d)!", mmx::net::Socket::AtoS(address), port, rc);
                            sel.Set(rc, mmx::net::S_EV_READ);
                        }
                    }

                    break;

                case S_SELECT:

                    rc = sel.Wait();

                    if (rc < 0)
                    {

                        tout = 1000;
                        state = S_WAIT;

                        if (rc != -ETIMEDOUT)
                        {

                            sel.Set(channel.Handle());
                            sel.Set(sock.Handle());

                            channel.Close();
                            sock.Close();

                            force_state = S_INIT;

                            mmx::logs::logE("(server) select error (rc = %d)!", rc);

                        }
                        else
                        {

                            force_state = S_SELECT;

                            mmx::logs::logW("(server) select timeout (rc = %d)!", rc);

                        }
                    }
                    else
                    {
                        state = S_ACCEPT;
                    }


                    break;

                case S_ACCEPT:

                    state = S_READ;

                    if (sel.IsRead(sock.Handle()))
                    {
                        mmx::net::Socket cli(sock);

                        if (cli.Handle() >= 0)
                        {

                            mmx::logs::logI("(server) client [%s:%d] accept success! (fd = %d)!", mmx::net::Socket::AtoS(cli.RemoteAddress()), cli.RemotePort(), cli.Handle());

                            // sel.Set(cli.Handle(), mmx::net::S_EV_READ);

                            clients.push_back(std::move(cli));

                        }
                        else
                        {
                            mmx::logs::logE("(server) client accept error! (rc = %d)!", cli.Handle());

                            sock.Close();

                            force_state = S_OPEN;
                            tout = 1000;
                            state = S_WAIT;
                        }
                    }

                    break;

                case S_READ:

                    state = S_SELECT;

                    if (sel.IsRead(channel.Handle()))
                    {
                        rc = channel.Read(buffer.data(), buffer.size());

                        if (rc > 0)
                        {
                            mmx::logs::logI("(server) pipe %s read %d bytes!", channel_name, rc);
                            state = S_WRITE;
                        }
                        else
                        {
                            mmx::logs::logE("(server) pipe %s read error! (rc = %d)!", channel_name, rc);

                            channel.Close();

                            force_state = S_INIT;
                            tout = 1000;
                            state = S_WAIT;
                        }
                    }

                    break;

                case S_CHECK: //проверка клиентов
                {
                            /*
                    auto r_it = clients.end();
                    const auto e_it = clients.end();

                    for (auto it = clients.begin(); it != e_it; it++)
                    {
                        auto& c = *it;

                        if(r_it != e_it)
                        {
                            clients.erase(r_it);
                            r_it = e_it;
                        }


                    }

                    if(r_it != e_it)
                    {
                        clients.erase(r_it);
                    }*/
                }
                break;

                case S_WRITE:

                    //mmx::logs::logD("(server) Write STATE!!!!");

                    {

                        auto r_it = clients.end();
                        const auto e_it = clients.end();

                        for (auto it = clients.begin(); it != e_it; it++)

                        {

                            if(r_it != e_it)
                            {
                                clients.erase(r_it);
                                r_it = e_it;
                            }

                            auto& c = *it;

                            rc = c.Send(buffer.data(), rc);


                            if (rc < 0)
                            {

                                 mmx::logs::logE("(server) client [%s:%d] write error! (rc = %d)!", mmx::net::Socket::AtoS(c.RemoteAddress()), c.RemotePort(), rc);

                                 r_it = it;

                                 //c.Close();

                            }
                            else
                            {

                                mmx::logs::logI("(server) client [%s:%d] write %d bytes!", mmx::net::Socket::AtoS(c.RemoteAddress()), c.RemotePort(), rc);

                            }
                        }

                        if(r_it != e_it)
                        {
                            clients.erase(r_it);
                        }

                    }

                    //std::remove_if(clients.begin(), clients.end(), [](const mmx::net::Socket& c){ return c->Handle() < 0; });
                    //std::remove(clients.begin(), clients.end(), '');



                    state = S_SELECT;

                    break;

                case S_WAIT:

                    if (tout > 0)
                        std::this_thread::sleep_for(std::chrono::milliseconds(tout));

                    mmx::logs::logI("(server) wait. force_state = %d", force_state);

                    state = force_state;

                    break;

                case S_ERR:

                    clients.clear();

                    sock.Close();

                    channel.Close();

                    state = S_EXIT;

                    break;

            }
        }

        return rc;

    }
}
