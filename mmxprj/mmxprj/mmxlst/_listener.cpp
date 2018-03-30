#include "defines.h"

#include "mmxlib/logs/log.h"
//#include "mmxlib/net/net.h"
#include "mmxlib/net/socket.h"
#include "mmxlib/net/select.h"
#include "mmxlib/net/portset.h"
#include "mmxlib/headers/udp.h"
#include "mmxlib/ipc/pchannel.h"
#include "mmxlib/staff/ipstaffer.h"
#include "mmxlib/staff/udpstaffer.h"
#include "mmxlib/tools/timer.h"

#include <functional>
#include <iostream>

#include "direction.h"
#include "fcntl.h"

#include <netdb.h>      // IPPROTO_UDP
#include <fcntl.h>      // O_NONBLOCK, O_RDWR
#include <errno.h>
#include <cstdio>       // sprintf



int listener(unsigned int addr, unsigned short begin, unsigned short end, int step)
{

    // test();

    int rc = -EINVAL;

    // для подсчета вызовов

    static int s_listener_id = 0;

    int pack_id = 0;

    int listener_id = ++s_listener_id;

    char buffer[1600];

    mmx::net::Socket sock(SOCK_RAW, IPPROTO_UDP);

    // канал для обмена с mmxcore

    mmx::ipc::PipeChannel pipe;

    // pipe.Open("/tmp/mmx0001", O_RDWR);

    mmx::net::PortSet ports;

    for (unsigned short p = begin; p < end; p += step)
    {
        ports.Set(p);
    }

    mmx::staff::DataPacket dpack(0x10000);

    dpack.Init(++pack_id);

    // направление, организующее выделение медиаданных и передачу mmx_core

    mmxlst::Direction dir(dpack, ports);

    // стаффер udp протокола

    mmx::staff::UDPStaffer udp_staffer(&dir);

    // стаффер ip-протокола

    mmx::staff::IPStaffer ip_staffer(&udp_staffer);

    // будем только слушать сокет

    mmx::net::Select sel;

    mmx::logs::logD("[listener(%d)] (addr=%d, port=%d, count=%d, step=%d) started!", listener_id, addr, begin, end, step);


    bool f_listen = true;

    mmx::net::timeout_t tout = mmx::net::INFINITE_TIMEOUT;

    mmx::tools::Timer sock_tim;

    mmx::tools::Timer pipe_tim;

    while (f_listen)
    {
        // априори
        tout = mmx::net::INFINITE_TIMEOUT;

        // сокет не открыт

        if (sock.Handle() < 0)
        {
            if (!sock_tim.IsStarted() || sock_tim.IsEnable())
            {

                sock_tim.Start(2000);

                rc = sock.Open(0,0,0,0,0, O_NONBLOCK);

                if (rc < 0)
                {


                    mmx::logs::logE("[listener(%d)] socket open error rc = %d", listener_id, rc);

                }
                else
                {
                    mmx::logs::logI("[listener(%d)] socket open seccess rc = %d", listener_id, rc);

                    sel.Set(rc, mmx::net::S_EV_READ);
                }
            }

        }

        // канал не открыт

        if (pipe.Handle() < 0)
        {
            if (!pipe_tim.IsStarted() || pipe_tim.IsEnable())
            {

                pipe_tim.Start(2000);

                rc = pipe.Open("/tmp/mmx0001", O_RDWR | O_NONBLOCK);

                if (rc < 0)
                {

                    mmx::logs::logE("[listener(%d)] pipe %s open error rc = %d", listener_id, pipe.Name(), rc);


                }
                else
                {

                    mmx::logs::logI("[listener(%d)] pipe %s open seccess rc = %d", listener_id, pipe.Name(), rc);



                    if (!dpack.IsEmpty())
                    {
                        // после успешного открытия канала, ожидаем событие по готовности передачи

                        sel.Set(pipe.Handle(), mmx::net::S_EV_WRITE);
                    }

                }
            }
        }

        if (sock.Handle() < 0 && sock_tim.IsStarted())
        {
            tout = sock_tim.Elapsed();
        }

        if (pipe.Handle() < 0 && pipe_tim.IsStarted())
        {
            auto elapse = pipe_tim.Elapsed();

            if (elapse < tout || tout == -1 && elapse > 0)
            {
                tout = elapse;
            }
        }

        if (!sel.IsEmpty())
        {

            rc = sel.Wait(tout);

            if (rc < 0)
            {

                if (rc == -ETIMEDOUT)
                {
                    mmx::logs::logW("[listener(%d)] select timeout rc = %d", listener_id, rc);
                }
                else
                {
                    mmx::logs::logE("[listener(%d)] select error rc = %d. Close channels...", listener_id, rc);

                    sel.Set(sock.Handle());
                    sel.Set(pipe.Handle());

                    sock.Close();
                    pipe.Close();
                }
            }
            else
            {
                if (sel.IsRead(sock.Handle()))
                {
                    // возможно пришли данные в сокетное соединение

                    rc = sock.Recv(buffer, sizeof(buffer));


                    if (rc < 0)
                    {
                        if (rc == EWOULDBLOCK || rc == EAGAIN)
                        {
                            // ситуация нормальная, но спорная! почему сработал select????

                            mmx::logs::logW("[listener(%d)] non-blocket forced reading socket %d. rc = %d", listener_id, sock.Handle(), rc);
                        }
                        else
                        {

                            mmx::logs::logE("[listener(%d)] socket =%d read error! rc = %d", listener_id, sock.Handle(), rc);


                            sel.Set(sock.Handle());

                            sock.Close();
                        }
                    }
                    else
                    {
                        //
                        ip_staffer.Dispatch(buffer, rc);



                        if (!dpack.IsEmpty())
                        {
                            // работать с передачей будем через select

                            sel.Set(pipe.Handle(), mmx::net::S_EV_WRITE);
                        }
                    }
                }

                if (sel.IsWrite(pipe.Handle()))
                {
                    // событие по наличию свободного канала


                    if (dpack.IsEmpty())
                    {
                        // ситуация непонятная. Сработали по записи, а данных в направлении нет...
                        // распечатаем ошибку.

                        mmx::logs::logE("[listener(%d)] No data for write!", listener_id);

                    }
                    else
                    {
                        auto pack = dpack.Data();

                        // данные получены - нужно их пересылать

                        rc = pipe.Write((const char*)pack, pack->header.length);

                        // ошибка передачи...

                        if (rc < 0)
                        {
                            if (rc == EWOULDBLOCK || rc == EAGAIN)
                            {
                                // ситуация нормальная, но опять спорная! почему сработал select????
                                // нужно распечатать

                                mmx::logs::logW("[listener(%d)] non-blocket forced reading pipe = %s. rc = %d", listener_id, pipe.Name(), rc);
                            }
                            else
                            {
                                mmx::logs::logE("[listener(%d)] pipe %s read error! rc = %d", listener_id, pipe.Name(), rc);

                                sel.Set(pipe.Handle());

                                pipe.Close();
                            }
                        }
                        else
                        {
                            mmx::logs::logI("[listener(%d)] pipe %s write %d bytes!", listener_id, pipe.Name(), rc);

                            // dir.Next();

                            dpack.Init(++pack_id);
                        }
                    }

                    // если нет данных на передачу, то и сигналить о возможности не нужно

                    if (dpack.IsEmpty())
                    {
                        sel.Set(pipe.Handle());
                    }
                }
            }
        }
        else
        {
            if (tout > 0)
            {
                mmx::logs::logW("[listener(%d)] sleep timeout = %d...", listener_id, tout);

                mmx::tools::Timer::Sleep(tout);
            }


        }
    }


    mmx::logs::logD("[listener(%d)] stop with rc = %d", listener_id, rc);

    return rc;
}
