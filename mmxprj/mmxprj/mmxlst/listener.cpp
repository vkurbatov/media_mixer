#include "listener.h"

#include <cstring>

#include <netdb.h>  // SOCK_RAW, IPPROTO_UDP
#include <fcntl.h>  // O_NONBLOCK
#include <errno.h>

#include "mmxlib/logs/log.h"

#define CHANNEL_PATTERN "/tmp/pmmxlst-%d"
#define DEFAULT_TIMEOUT 2000

namespace mmxlst
{

    Listener::Listener(mmx::net::address_t address, unsigned char channel, mmx::net::PortSet& ports) :
        ports_(ports),
        socket_(SOCK_RAW, IPPROTO_UDP),
        address_(address),
        channel_(channel),
        datapack_(0x10000),
        direction_(datapack_, ports_),
        udp_sniffer_(&direction_),
        ip_sniffer_(&udp_sniffer_),
        pack_id_(0)
    {
        mmx::logs::logD("[%p] Listener::Listener(%d, %d, [ports])", this, address, channel);
    }

    Listener::~Listener()
    {
        mmx::logs::logD("[%p] Listener::~Listener(begin)", this);

        clear();

        mmx::logs::logD("[%p] Listener::~Listener(end)", this);

    }

    int Listener::Execute()
    {
        mmx::logs::logD("[%p] Listener::Execute(begin)", this);

        int rc = initialization();

        while(rc >= 0)
        {
            checkConnection();
            setTimeout();
            if (waitEvents() > 0)
            {
                readData();
                writeData();
            }
        }
        clear();

        mmx::logs::logD("[%p] Listener::Execute(stop)", this);
    }

    int Listener::initialization()
    {
        mmx::logs::logD("[%p] Listener::initialization(begin)", this);


        datapack_.Init(++pack_id_);
        sock_timer_.Start(0);
        pipe_timer_.Start(0);

        mmx::logs::logD("[%p] Listener::initialization(stop)", this);

        return 0;
    }

    int Listener::checkConnection()
    {
        int rc = 0;

        char pipe_name_[sizeof(CHANNEL_PATTERN)+4];

        std::sprintf(pipe_name_, CHANNEL_PATTERN, (int)channel_);

        // проверка сокетного соединения

        if (socket_.Handle() < 0)
        {
            if (sock_timer_.IsEnable())
            {
                rc = socket_.Open(0, 0, 0, 0, 0, O_NONBLOCK);

                if (rc >= 0)
                {
                    mmx::logs::logI("[%p] Listener::checkConnection() socket %d open success!", this, rc);
                    select_.Set(rc, mmx::net::S_EV_READ);
                }
                else
                {
                    mmx::logs::logE("[%p] Listener::checkConnection() socket open error = %d!", this, rc);
                }

                sock_timer_.Start(DEFAULT_TIMEOUT);
            }
        }

        // проверка pipe

        if (pipe_.Handle() < 0)
        {
            if (pipe_timer_.IsEnable())
            {
                rc = pipe_.Open(pipe_name_, O_RDWR);

                if (rc >= 0)
                {
                    mmx::logs::logI("[%p] Listener::checkConnection() pipe %d (%s) open success!", this, rc, pipe_name_);

                    checkWrite();

                }
                else
                {
                    mmx::logs::logE("[%p] Listener::checkConnection() pipe %s open error = %d!", this, pipe_name_, rc);
                }

                pipe_timer_.Start(DEFAULT_TIMEOUT);
            }
        }

    }

    int Listener::setTimeout()
    {
        int tout = mmx::net::INFINITE_TIMEOUT;

        // TODO: костыль... мне не нравится....

        if (socket_.Handle() < 0)
        {
            auto elapse = sock_timer_.Elapsed();
            if (sock_timer_.IsEnable())
            {
                if (tout > elapse)
                {
                    tout = elapse < 0 ? 0 : elapse;
                }
            }
        }


        if (pipe_.Handle() < 0)
        {
            auto elapse = pipe_timer_.Elapsed();
            if (sock_timer_.IsEnable())
            {
                if (tout > elapse)
                {
                    tout = elapse < 0 ? 0 : elapse;
                }
            }
        }

        timeout_ = tout;

        return timeout_;
    }

    int Listener::waitEvents()
    {

        int rc = 0;

        if (select_.IsEmpty())
        {
            mmx::tools::Timer::Sleep(timeout_);
        }
        else
        {

            rc = select_.Wait(timeout_);

            if (rc < 0)
            {
                if (rc == -ETIMEDOUT)
                {

                    mmx::logs::logW("[%p] Listener::waitEvents() select timeout rc = %d", this, rc);
                    rc = 0;
                }
                else
                {
                    mmx::logs::logE("[%p] Listener::waitEvents() select error rc = %d", this, rc);

                    clear();
                }
            }
        }

        return rc;
    }


    int Listener::checkWrite()
    {
        if (!datapack_.IsEmpty())
        {
            select_.Set(pipe_.Handle(), mmx::net::S_EV_WRITE);
        }

        return 0;
    }

    int Listener::readData()
    {
        int rc = 0;

        if (select_.IsRead(socket_.Handle()))
        {
            rc = socket_.Recv(buffer_, sizeof(buffer_));

            if (rc < 0)
            {
                if (rc == EWOULDBLOCK || rc == EAGAIN)
                {
                    // ситуация нормальная, но спорная! почему сработал select????

                    mmx::logs::logW("[%p] Listener::readData() non-blocket forced reading socket %d. rc = %d", this, socket_.Handle(), rc);
                }
                else
                {

                    mmx::logs::logE("[%p] Listener::readData() socket =%d read error! rc = %d", this, socket_.Handle(), rc);

                    select_.Set(socket_.Handle());

                    socket_.Close();
                }
            }
            else
            {
                ip_sniffer_.Dispatch(buffer_, rc);

                checkWrite();

            }
        }

        return rc;
    }

    int Listener::writeData()
    {
        int rc = 0;

        if (select_.IsWrite(pipe_.Handle()))
        {
            // событие по наличию свободного канала

            if (datapack_.IsEmpty())
            {
                // ситуация непонятная. Сработали по записи, а данных в направлении нет...
                // распечатаем ошибку.

                mmx::logs::logE("[%p] Listener::writeData() No data for write!", this);

            }
            else
            {
                auto pack = datapack_.Data();

                // данные получены - нужно их пересылать

                rc = pipe_.Write((const char*)pack, pack->header.length);

                // ошибка передачи...

                if (rc < 0)
                {
                    if (rc == EWOULDBLOCK || rc == EAGAIN)
                    {
                        // ситуация нормальная, но опять спорная! почему сработал select????
                        // нужно распечатать

                        mmx::logs::logW("[%p] Listener::writeData() non-blocket forced reading pipe = %s. rc = %d", this, pipe_.Name(), rc);
                    }
                    else
                    {
                        mmx::logs::logE("[%p] Listener::writeData() pipe %s read error! rc = %d", this, pipe_.Name(), rc);

                        select_.Set(pipe_.Handle());

                        pipe_.Close();
                    }
                }
                else
                {
                    mmx::logs::logI("[%p] Listener::writeData() pipe %s write %d bytes!", this, pipe_.Name(), rc);

                    // dir.Next();

                    datapack_.Init(++pack_id_);
                }
            }

            // если нет данных на передачу, то и сигналить о возможности не нужно

            if (datapack_.IsEmpty())
            {
                select_.Set(pipe_.Handle());
            }
        }

        return rc;
    }

    int Listener::clear()
    {
        select_.Reset();
        socket_.Close();
        pipe_.Close();
    }
}
