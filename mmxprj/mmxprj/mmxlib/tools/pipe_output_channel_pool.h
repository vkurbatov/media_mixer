#ifndef _MMX_TOOLS_PIPE_OUTPUT_CHANNEL_POOL_H
#define _MMX_TOOLS_PIPE_OUTPUT_CHANNEL_POOL_H

#include "pipe_output_channel.h"

#include <list>

namespace mmx
{
    namespace tools
    {
        class PipeOutputChannelPool
        {

            PipeOutputChannel*              channels_[0x100];
            std::list<PipeOutputChannel>    channel_list_;


            char                            pipe_name_prefix_[256];

        public:

            PipeOutputChannelPool(const char* pipe_name_prefix);
            ~PipeOutputChannelPool();

            PipeOutputChannel* GetChannel(unsigned char channel, net::SelectExtension& select, int interval = 2000);
            PipeOutputChannel* operator[] (unsigned char idx);
            int ReleaseChannel(unsigned char channel);
            std::list<PipeOutputChannel>& GetChannels();

            void Clear();
        };
    }
}

#endif
