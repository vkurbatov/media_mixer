#include "pipe_output_channel.h"

#include <cstring>

#include <fcntl.h>  // O_NONBLOCK, ORWRD
#include <errno.h>

namespace mmx
{
    namespace tools
    {
        PipeOutputChannel::PipeOutputChannel(const char* pipe_name, mmx::net::SelectExtension& select, int interval) :
            select_(select),
            writer_(pipe_, 10),
            interval_(interval)
        {
            std::memset(pipe_name_, 0, sizeof(pipe_name_));

            if (pipe_name != nullptr && *pipe_name != '\0')
            {
                std::strcpy(pipe_name_, pipe_name);
            }

            timer_.Start(0);
        }

        PipeOutputChannel::PipeOutputChannel(PipeOutputChannel&& channel) :
            pipe_(std::move(channel.pipe_)),
            writer_(pipe_, 10),
            timer_(std::move(channel.timer_)),
            select_(channel.select_),
            interval_(channel.interval_)
        {

            std::memcpy(pipe_name_, channel.pipe_name_, sizeof(pipe_name_));
            std::memset(channel.pipe_name_, 0, sizeof(channel.pipe_name_));

        }

        PipeOutputChannel::~PipeOutputChannel()
        {
            Close();
        }

        int PipeOutputChannel::Dispatch()
        {
            checkConnect();
            checkRead();
            int rc = defferedWrite();

            return rc;
        }

        int PipeOutputChannel::GetLeftTimeWork() const
        {
            return pipe_.Handle() < 0 && timer_.IsStarted()
                    ? timer_.Left()
                    : mmx::net::INFINITE_TIMEOUT;
        }

        int PipeOutputChannel::Close()
        {
            writer_.Drop();

            if (pipe_.Handle() >= 0)
            {
                select_.Set(pipe_.Handle());
                return pipe_.Close();
            }

            return 0;
        }

       /* void PipeOutputChannel::Reset()
        {
            writer_.Reset();
        }*/

        bool PipeOutputChannel::IsDown() const
        {
            return pipe_.Handle() < 0;
        }

        int PipeOutputChannel::PutData(const void* data, int size)
        {
            int rc = 0;

            rc = writer_.Write(data, size);

            if (rc < 0 && rc != -EAGAIN)
            {
                Close();
            }
            else
            {
                if (!writer_.IsEmpty())
                {
                    int fd = pipe_.Handle();

                    if (fd >= 0)
                    {
                        select_.SetWrite(fd);
                    }
                }
            }

            return rc;
        }

        void PipeOutputChannel::Drop()
        {
            writer_.Drop();
        }

        int PipeOutputChannel::checkConnect()
        {
            int rc = 0;

            if (timer_.IsEnable())
            {

                if (pipe_.Handle() < 0)
                {
                    rc = pipe_.Open(pipe_name_, O_WRONLY | O_NONBLOCK);

                    if (rc > 0)
                    {
                        select_.SetRead(rc);


                        if (!writer_.IsEmpty())
                        {
                            select_.SetWrite(rc);
                        }
                    }
                }

                timer_.Start(interval_);
            }

            return rc;
        }

        int PipeOutputChannel::checkRead()
        {
            int rc = 0;

            int fd = pipe_.Handle();

            if (fd >= 0)
            {
                if (select_.IsRead(fd))
                {
                    Close();
                }
            }

            return rc;
        }

        int PipeOutputChannel::defferedWrite()
        {
            int rc = 0;

            int fd = pipe_.Handle();

            if (fd >= 0)
            {
                if (select_.IsWrite(fd))
                {

                    if (!writer_.IsEmpty())
                    {

                        rc = writer_.Write(nullptr, 0);

                        if (rc < 0 && rc != -EAGAIN)
                        {
                            Close();
                        }
                        else
                        {
                            if (writer_.IsEmpty())
                            {
                                select_.ClrWrite(fd);
                            }
                        }
                    }

                }
            }

            return rc;
        }
    }
}
