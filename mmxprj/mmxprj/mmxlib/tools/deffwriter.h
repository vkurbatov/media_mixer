#ifndef _MMX_TOOLS_DEFFERED_WRITER_H
#define _MMX_TOOLS_DEFFERED_WRITER_H

#include <vector>
#include <queue>

#include "ipc/io.h"
#include "staff/packet.h"

namespace mmx
{
    namespace tools
    {

        //const int MAX_PACKET_SIZE = 1500; // mtu

        class DeferredWriter : ipc::IIO
        {

            ipc::IIO&                   io_;
            std::queue<staff::Packet>   q_write_;
            std::queue<staff::Packet>   q_free_;
            int                         limit_;

            //std::vector<char>           rd_buff_;

        public:

            DeferredWriter(ipc::IIO& io, int limit);

            // IIO
            int Write(const char* data, int size, int flags = 0);
            int Read(char* data, int size, int flags = 0);

            bool IsEmpty() const;
            int Count() const;

            int Drop(int count = 0);

        private:

            staff::Packet* getPacket(int size = 0);
            void pushData(const char* data, int size);
            void pop();

        };
    }
}

#endif
