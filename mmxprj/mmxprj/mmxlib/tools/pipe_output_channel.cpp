#include "pipe_output_channel.h"

#include <cstring>

#include <fcntl.h>  // O_NONBLOCK, ORWRD
#include <errno.h>

#include <string>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("PipeOutputChannel", msg)

namespace mmx
{
    namespace tools
    {
        PipeOutputChannel::PipeOutputChannel(const char* pipe_name_prefix, unsigned char channel, mmx::net::SelectExtension& select, int interval) :
            select_(select),
            writer_(pipe_, 10),
            interval_(interval),
            channel_(channel),
            pack_id_(0),
            data_(0x10000),
            dp_writter_(data_.data(), data_.size()),
            pipe_((std::string(pipe_name_prefix) + std::to_string((int)channel)).c_str(),O_WRONLY | O_NONBLOCK)
        {

            dp_writter_.BuildPacket(++pack_id_);

            timer_.Start(0);
        }

        PipeOutputChannel::PipeOutputChannel(PipeOutputChannel&& channel) :
            pipe_(std::move(channel.pipe_)),
            writer_(pipe_, 10),
            timer_(std::move(channel.timer_)),
            data_(std::move(channel.data_)),
            dp_writter_(std::move(channel.dp_writter_)),
            select_(channel.select_),
            interval_(channel.interval_),
            channel_(channel.channel_),
            pack_id_(channel.pack_id_)
        {


        }

        PipeOutputChannel::~PipeOutputChannel()
        {
            Close();
        }

        int PipeOutputChannel::Dispatch(dispatch_flags_t dispatch, void* context)
        {
            checkConnect();
            checkRead();
            int rc = defferedWrite();

            return rc;
        }

        int PipeOutputChannel::QueryOrderTimeout() const
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

        data::DataPacketWriter& PipeOutputChannel::GetWritter()
        {
            return dp_writter_;
        }

        unsigned char PipeOutputChannel::GetChannelId() const
        {
            return channel_;
        }

        bool PipeOutputChannel::IsDown() const
        {
            return pipe_.Handle() < 0;
        }

        bool PipeOutputChannel::IsReadyData() const
        {
            return false;
        }


        int PipeOutputChannel::Send()
        {
            int rc = 0;

            headers::DATA_PACK& dp = *(headers::DATA_PACK*)data_.data();

            if (dp.header.block_count > 0)
            {
                rc = writer_.Write(data_.data(), dp.header.length);

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

                dp_writter_.BuildPacket(++pack_id_);
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
                    rc = pipe_.Open();

                    if (rc > 0)
                    {
                        DLOGI(LOG_BEGIN("checkConnect(): output pipe %s create success, fd = %d"), pipe_.Name(), rc);

                        select_.SetRead(rc);

                        if (!writer_.IsEmpty())
                        {
                            select_.SetWrite(rc);
                        }
                    }
                    else
                    {
                        DLOGW(LOG_BEGIN("checkConnect(): output pipe %s is not create, rc = %d"), pipe_.Name(), rc);
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
                            DLOGE(LOG_BEGIN("defferedWrite(): deffered write error, rc = %d"), rc);
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
