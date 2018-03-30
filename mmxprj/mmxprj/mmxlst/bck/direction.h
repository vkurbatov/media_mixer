#ifndef _MMXLST_DIRECTION_H
#define _MMXLST_DIRECTION_H

#include <vector>

#include "channel.h"

namespace mmxlst
{
    class Direction
    {
        int direction_id_;
        int begin_;
        int end_;
        int step_;
        //std::vector<Channel> channels_;

        //static std::vector<Direction*> s_directions_;

        int port2idx(unsigned short port) const;
    public:
        Direction(unsigned short begin, unsigned short end, int step, int direction_id);
        ~Direction();
        /*static int BuildDirection(unsigned short begin, unsigned short end, int step = 1);
        static int DestroyDirection(int direction_id);
        static Direction& GetDirection(int direction_id);*/
        int Dispatch(int sock);
        //int Process(void(*call)(unsigned short, mmx::staff::Packet&&, void*), void* context = nullptr);
        int Count(unsigned short port) const;
        //int Pop(unsigned short port, mmx::staff::Packet&& packet);
    };
}

#endif
