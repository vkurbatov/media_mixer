#include "direction.h"

#include "mmxlib/logs/log.h"
#include "mmxlib/net/net.h"
#include "mmxlib/headers/udp.h"

#include <vector>
#include <mutex>

#include <memory>       // move
#include <algorithm>    // find, distance

#include <sys/socket.h> // htons
#include <netdb.h>      // IPPROTO_UDP
#include "errno.h"



namespace mmxlst
{
    static std::mutex g_mutex;

    static std::vector<Direction*> s_directions_;

    Direction::Direction(unsigned short begin, unsigned short end, int step, int direction_id)
        : direction_id_(direction_id), port_(begin), step_(step), channels_()
    {

        int port = begin;

        while (port <= end)
        {
            channels_.push_back(Channel(port, direction_id));

            port += step;
        }

        mmx::logs::logD("Direction(begin = %d, end = %d, step = %d)[%d] Created with %d channels", begin, end, step, direction_id_, channels_.size());
    }

    Direction::~Direction()
    {
        mmx::logs::logD("~Direction(port_ = %d)[%d] Destroy with %d channels", port_, direction_id_, channels_.size());
    }

    int Direction::BuildDirection(unsigned short begin, unsigned short end, int step)
    {
        std::unique_lock<std::mutex> lock(g_mutex);

        int rc = -EINVAL;

        if (begin < end)
        {
            // попрбуем найти свободный элемент в дескрипторной таблице

            auto it = std::find(s_directions_.begin(),s_directions_.end(), nullptr);

            auto idx = std::distance(s_directions_.begin(), s_directions_.end());

            std::unique_ptr<Direction> dir(new Direction(begin, end, step, idx));

            if (idx >= s_directions_.size())
            {
                // пустых записей нет, будет новая запись
                s_directions_.push_back(dir.release());
            }
            else
            {
                // записываем в указатель в свободную ячейку

                s_directions_[idx] = dir.release();
            }
            rc = idx;

        }

        return rc;
    }

    int Direction::DestroyDirection(int direction_id)
    {
        std::unique_lock<std::mutex> lock(g_mutex);

        int rc = -EINVAL;

        if (direction_id < s_directions_.size())
        {
            if (s_directions_[direction_id] != nullptr)
            {
                delete s_directions_[direction_id];
                s_directions_[direction_id] = nullptr;
                rc = direction_id;
            }
        }

        return rc;
    }

    Direction& Direction::GetDirection(int direction_id)
    {
        std::unique_lock<std::mutex> lock(g_mutex);

        if (direction_id < s_directions_.size() && s_directions_[direction_id] != nullptr)
        {
            return *s_directions_[direction_id];
        }

        throw("Failed direction id");
    }

    int Direction::Dispatch(int sock)
    {

        mmx::headers::UDPIP4 &udp = *(mmx::headers::UDPIP4*)buff_;

        int rc = mmx::net::read_sock(sock, buff_, sizeof(buff_));

        if (rc >= 0)
        {
            mmx::logs::logD("Dispatch(sock = %d)[%d] reading %d bytes", sock, direction_id_, rc);
            if (rc >= sizeof(udp))
            {
                if (udp.ip4_header.protocol == IPPROTO_UDP)

                {
                    unsigned short port = udp.udp_header.port_dst;// ::htons(udp.udp_header.port_dst);
                    int idx = port2idx(port);

                    if (idx >= 0)
                    {
                        rc = channels_[idx].Push(Packet(buff_ + sizeof(udp), udp.udp_header.length));
                    }
                    else
                    {
                        mmx::logs::logW("Dispatch(sock = %d)[%d] port not found (%d)", sock, direction_id_, port);
                    }
                }
                else
                {
                    mmx::logs::logW("Dispatch(sock = %d)[%d] prorocol is not UDP (%d)", sock, direction_id_, udp.ip4_header.protocol);
                }
            }
            else
            {
                 mmx::logs::logW("Dispatch(sock = %d)[%d] udp packet size too short", sock, direction_id_);
            }
        }
        else
        {
             mmx::logs::logE("Dispatch(sock = %d)[%d] reading error = %d", sock, direction_id_, rc);
        }
    }

    int Direction::Process(void(*call)(unsigned short, Packet&&, void*), void* context)
    {
        int rc = -EINVAL;

        if (call != nullptr)
        {
            rc = 0;

            for (auto& c : channels_)
            {

                while(!c.IsEmpty())
                {
                    Packet pack;
                    c.Pop(std::move(pack));
                    call(c.Port(),std::move(pack), context);
                }
            }
        }

        return rc;
    }

    int Direction::Count(unsigned short port) const
    {
        int rc = -EINVAL;

        int idx = port2idx(port);

        if (idx >= 0)
        {
            rc = channels_[idx].PacketCount();
        }

        return rc;
    }

    int Direction::Pop(unsigned short port, Packet&& packet)
    {
        int rc = -EINVAL;

        int idx = port2idx(port);

        if (idx >= 0)
        {
            rc = channels_[idx].Pop(std::move(packet));
        }

        return rc;
    }

    int Direction::port2idx(unsigned short port) const
    {
        int rc = -EINVAL;

        if ((port_ - port) % step_ == 0)
        {
            int idx = ((int)port - (int)port_) / step_;

            if (idx >= 0 && idx < channels_.size())
            {
                rc = idx;
            }
        }

        return rc;
    }

}
