#ifndef _MMXLST_DIRECTION_H
#define _MMXLST_DIRECTION_H

#include <vector>

#include "channel.h"

namespace mmxlst
{
    class Direction
    {
        static const int DEFAULT_BUFFER_SIZE = 1600; // MTU + запас

        int direction_id_;
        int port_;
        int step_;
        std::vector<Channel> channels_;

        packet_data_t buff_[DEFAULT_BUFFER_SIZE];

        //static std::vector<Direction*> s_directions_;


        Direction(unsigned short begin, unsigned short end, int step, int direction_id);
        int port2idx(unsigned short port) const;
    public:
        ~Direction();
        static int BuildDirection(unsigned short begin, unsigned short end, int step = 1);
        static int DestroyDirection(int direction_id);
        static Direction& GetDirection(int direction_id);
        int Dispatch(int sock);
        int Process(void(*call)(unsigned short, Packet&&, void*), void* context = nullptr);
        int Count(unsigned short port) const;
        int Pop(unsigned short port, Packet&& packet);
    };
}

#endif
