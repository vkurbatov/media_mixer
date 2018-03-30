#include <memory>
#include "channel.h"

#include "mmxlib/net/net.h"
#include "mmxlib/logs/log.h"

namespace mmxlst
{
    int Channel::s_channel_id_ = 0;

    Channel::Channel() :
        parent_id_(-1),
        port_(0),
        channel_id_(-1),
        total_size_(0)

    {
        mmx::logs::logD("Channel()[%d]", channel_id_);
    }

    Channel::Channel(unsigned short port, int parent_id) :
        parent_id_(parent_id),
        port_(port),
        channel_id_(++s_channel_id_),
        total_size_(0)
    {
        mmx::logs::logD("Channel(port = %d, parent_id = %d)[%d]", port, parent_id, channel_id_);
    }

    Channel::Channel(Channel&& channel) :
        parent_id_(channel.parent_id_),
        port_(channel.port_),
        channel_id_(channel.channel_id_),
        total_size_(channel.total_size_),
        queue_(std::move(channel.queue_))
    {
        channel.port_ = 0;
        channel.parent_id_ = -1;
        channel.channel_id_ = -1;

        mmx::logs::logD("Channel(channel = %p)[%d]", &channel, channel_id_);
    }

    Channel::~Channel()
    {
         mmx::logs::logD("~Channel(port = %d)[%d:%d]", port_, parent_id_, channel_id_);
    }


    int Channel::Push(mmx::staff::Packet&& packet)
    {

        auto sz = packet.Size();
        total_size_ += sz;
        queue_.push(std::move(packet));
        return sz;

    }

    /*
    int Channel::Read(int sock)
    {
        int rc = mmx::net::read_sock(sock, buff_, sizeof(buff_));
        if (rc < 0)
        {
            mmx::logs::logE("Channel::Read(sock = %d) [%d:%d] reading error %d", sock, parent_id_, channel_id_, rc);
        }
        else
        {
            mmx::logs::logI("Channel::Read(sock = %d) [%d:%d] reading %d bytes!", sock, parent_id_, channel_id_, rc);
            if (rc > 0)
            {
                queue_.push(Packet(buff_, rc));
                total_size_ += rc;
            }
        }

        return rc;
    }
    */

    int Channel::PacketCount() const
    {
        return queue_.size();
    }

    int Channel::TotalSize() const
    {
        return total_size_;
    }

    int Channel::Pop(mmx::staff::Packet&& packet)
    {
        int rc = -ENODATA;
        if(!queue_.empty())
        {
            packet = std::move(queue_.front());
            total_size_ -= queue_.size();
            queue_.pop();
            rc = queue_.size();
            mmx::logs::logE("Channel::Pop(Packet = %p) [%d:%d] Pop Packet(data = %p, size = %d)", &packet, parent_id_, channel_id_, packet.Data(), packet.Size());
        }
        else
        {
            mmx::logs::logE("Channel::Pop(Packet = %p) [%d:%d] Pop error = %d", &packet, parent_id_, channel_id_, rc);
        }
        return rc;
    }

    bool Channel::IsEmpty() const
    {
        return queue_.empty();
    }

    unsigned short Channel::Port() const
    {
        return port_;
    }

    int Channel::Id() const
    {
        return channel_id_;
    }

    int Channel::ParentId() const
    {
        return parent_id_;
    }

}
