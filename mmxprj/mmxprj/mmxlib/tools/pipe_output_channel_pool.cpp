#include "pipe_output_channel_pool.h"

#include <cstring>

// #include <algorithm>


namespace mmx
{
    namespace tools
    {

        PipeOutputChannelPool::PipeOutputChannelPool(const char* pipe_name_prefix)
        {
            std::memset(pipe_name_prefix_, 0, sizeof(pipe_name_prefix_));
            std::memset(pipe_name_, 0, sizeof(pipe_name_));
            std::memset(channels_, 0, sizeof(channels_));
            if (pipe_name_prefix != nullptr && *pipe_name_prefix != '\0')
            {
                std::strcpy(pipe_name_prefix_, pipe_name_prefix);
            }
        }

        PipeOutputChannelPool::~PipeOutputChannelPool()
        {

        }

        PipeOutputChannel* PipeOutputChannelPool::GetChannel(unsigned char channel, net::SelectExtension& select, int interval)
        {
            PipeOutputChannel* rc = channels_[channel];


            if (rc == nullptr)
            {
                std::sprintf(pipe_name_, pipe_name_prefix_, (int)channel);

                channel_list_.push_back(std::move(PipeOutputChannel(pipe_name_,select, interval)));

                channels_[channel] = &channel_list_.back();
            }

            return rc;
        }

        int PipeOutputChannelPool::ReleaseChannel(unsigned char channel)
        {
            int rc = 0;

            auto c = channels_[channel];

            if (c != nullptr)
            {

                channel_list_.remove_if([c](const PipeOutputChannel& cc){ return &cc == c;});

                channels_[channel] = nullptr;

            }

            return rc;
        }

        const std::list<PipeOutputChannel>& PipeOutputChannelPool::GetChannels()
        {
            return channel_list_;
        }

        void PipeOutputChannelPool::Clear()
        {

            channel_list_.clear();
            std::memset(channels_, 0, sizeof(channels_));

        }
    }
}
