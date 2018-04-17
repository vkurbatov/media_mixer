#include "pipe_input_channel.h"

#include <cstring>

#include <fcntl.h>  // O_NONBLOCK, ORWRD
#include <errno.h>

namespace mmx
{
    namespace tools
    {
        PipeInputChannel::PipeInputChannel(const char* pipe_name_prefix, unsigned char channel, mmx::net::SelectExtension& select, int interval) :
            select_(select),
            data_(0x10000),
            interval_(interval),
            read_bytes_(0),
            channel_(channel)
        {
            std::memset(pipe_name_, 0, sizeof(pipe_name_));

            if (pipe_name_prefix != nullptr && *pipe_name_prefix != '\0')
            {
                //std::strcpy(pipe_name_, pipe_name);
                std::sprintf(pipe_name_, pipe_name_prefix, channel);
            }

            timer_.Start(0);
        }

        PipeInputChannel::PipeInputChannel(PipeInputChannel&& channel) :
            pipe_(std::move(channel.pipe_)),
            timer_(std::move(channel.timer_)),
            data_(std::move(channel.data_)),
            select_(channel.select_),
            interval_(channel.interval_),
            read_bytes_(channel.read_bytes_),
            channel_(channel.channel_)
        {

            std::memcpy(pipe_name_, channel.pipe_name_, sizeof(pipe_name_));
            std::memset(channel.pipe_name_, 0, sizeof(channel.pipe_name_));

        }

        PipeInputChannel::~PipeInputChannel()
        {
            Close();
        }

        int PipeInputChannel::Dispatch()
        {
            checkConnect();
            readData();
        }

        int PipeInputChannel::GetLeftTimeWork() const
        {
            return pipe_.Handle() < 0 && timer_.IsStarted()
                    ? timer_.Left()
                    : mmx::net::INFINITE_TIMEOUT;
        }

        int PipeInputChannel::Close()
        {

            if (pipe_.Handle() >= 0)
            {
                select_.Set(pipe_.Handle());
                return pipe_.Close();
            }

            return 0;
        }

        bool PipeInputChannel::IsDown() const
        {
            return pipe_.Handle() < 0;
        }

        const void* PipeInputChannel::Data() const
        {
            return data_.data();
        }

        int PipeInputChannel::Size() const
        {
            return read_bytes_;
        }

        void PipeInputChannel::Drop()
        {

            if (read_bytes_ >= data_.size())
            {
                select_.SetRead(pipe_.Handle());
            }

            read_bytes_ = 0;
        }

        int PipeInputChannel::checkConnect()
        {
            int rc = 0;

            if (timer_.IsEnable())
            {

                if (pipe_.Handle() < 0)
                {
                    rc = pipe_.Open(pipe_name_, O_RDONLY | O_NONBLOCK, 0777);

                    if (rc > 0)
                    {
                        if (read_bytes_ < data_.size())
                        {
                            select_.SetRead(rc);
                        }
                    }
                }

                timer_.Start(interval_);
            }

            return rc;
        }

        int PipeInputChannel::readData()
        {
            int rc = 0;

            int fd = pipe_.Handle();

            if (fd >= 0)
            {
                if (select_.IsRead(fd))
                {
                    int lost_bytes = data_.size() - read_bytes_;
                    if (lost_bytes > 0)
                    {

                        rc = pipe_.Read(data_.data() + read_bytes_, lost_bytes);

                        if (rc >= 0)
                        {
                            read_bytes_ += rc;
                        }
                        else
                        {
                            switch (rc)
                            {
                                case -EAGAIN:

                                    // ничего не делаем

                                    break;
                                default:

                                    Close();
                                    fd = -1;

                            }
                        }
                    }

                    if (read_bytes_ >= data_.size())
                    {
                        select_.ClrRead(fd);
                    }
                }
            }

            return rc;
        }
    }
}
