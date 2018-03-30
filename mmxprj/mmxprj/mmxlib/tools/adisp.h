#ifndef _MMX_TOOLS_ADISP_H
#define _MMX_TOOLS_AISP_H

#include <vector>
#include <queue>

#include "ipc/io.h"
#include "staff/packet.h"

namespace mmx
{
    namespace tools
    {

        //const int MAX_PACKET_SIZE = 1500; // mtu

        class ADispatcher : public ipc::IIO
        {
            std::queue<staff::Packet>   q_write_;
            std::queue<staff::Packet>   q_read_;

            //std::vector<char>           rd_buff_;

        public:

            ADispatcher();
            virtual ~ADispatcher() {};

            // IIO
            int Write(const char* data, int size, int flags = 0);
            int Read(char* data, int size, int flags = 0);

            bool IsWriteEmpty() const;
            bool IsReadEmpty() const;

            void Reset();

        protected:

            virtual ipc::IIO& getIO() = 0;
        };
    }
}

#endif
