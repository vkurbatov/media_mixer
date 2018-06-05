#include "pipe_input_channel.h"

#include <cstring>

#include <fcntl.h>  // O_NONBLOCK, ORWRD
#include <errno.h>

#include <string>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("PipeInputChannel", msg)

namespace mmx
{
    namespace tools
    {
        PipeInputChannel::PipeInputChannel(const char* pipe_name_prefix, unsigned char channel, mmx::net::SelectExtension& select, int interval) :
            select_(select),
            data_(0x10000),
            interval_(interval),
            read_bytes_(0),
            channel_(channel),
            pipe_(((std::string(pipe_name_prefix) + std::to_string((int)channel)).c_str()), O_RDONLY | O_NONBLOCK, 0666)
        {

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



        }

        PipeInputChannel::~PipeInputChannel()
        {
            Close();
        }

        int PipeInputChannel::Dispatch(dispatch_flags_t dispatch, void* context)
        {
            checkConnect();
            readData();
        }

        int PipeInputChannel::QueryOrderTimeout() const
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

        bool PipeInputChannel::IsReadyData() const
        {
            return read_bytes_ > 0;
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

                    rc = pipe_.Open();

                    if (rc > 0)
                    {
                        DLOGI(LOG_BEGIN("checkConnect(): input pipe %s create success, fd = %d"), pipe_.Name(), rc);

                        if (read_bytes_ < data_.size())
                        {
                            select_.SetRead(rc);
                        }
                        else
                        {
                            DLOGW(LOG_BEGIN("checkConnect(): input pipe %s is not create, rc = %d"), pipe_.Name(), rc);
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

                        if (rc > 0)
                        {

                            read_bytes_ += rc;
                        }
                        else
                        {
                            switch (rc)
                            {
                                case -EAGAIN:
                                    DLOGW(LOG_BEGIN("readData(): would block read, rc = %d"), rc);
                                    Close();
                                    fd = -1;
                                    // ничего не делаем

                                    break;
                                default:
                                    DLOGE(LOG_BEGIN("readData(): error read data, rc = %d"), rc);
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
