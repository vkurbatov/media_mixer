#ifndef _MMX_TOOLS_DEFERRED_WRITER_H
#define _MMX_TOOLS_DEFERRED_WRITER_H

#include <vector>
#include <queue>

#include "ipc/io.h"
#include "data/packet.h"

namespace mmx
{
    namespace tools
    {

        //const int MAX_PACKET_SIZE = 1500; // mtu

        class DeferredWriter : ipc::IIO
        {

            ipc::IIO&                   io_;
            std::queue<data::Packet>    q_write_;
            std::queue<data::Packet>    q_free_;
            int                         limit_;
            int                         last_result_;
            int                         lost_;
            //std::vector<char>           rd_buff_;

        public:

            DeferredWriter(ipc::IIO& io, int limit = 0);

            // IIO
            int Write(const void* data, int size, int flags = 0);
            int Read(void* data, int size, int flags = 0);
            bool IsCanWrite() const override;
            bool IsCanRead() const override;

            bool IsEmpty() const;
            int Count() const;

            int Drop(int count = 0);
            void Reset();
            int LastResult() const;
            int Lost() const;

        private:

            data::Packet* getPacket(int size = 0);
            void pushData(const char* data, int size);
            void pop();


        };
    }
}

#endif
