#include "pchannel.h"

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>      // remove
#include <fcntl.h>      // open
#include <unistd.h>     // close
#include <cstdarg>      // работа с переменнам количеством аргументов
#include <cstring>      // strcpy

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("PipeChannel", msg)

namespace mmx
{
    namespace ipc
    {
        PipeChannel::PipeChannel(const char *pipe_name, int mode, int access) :
            handle_(-1),
            mode_(mode),
            access_(access)
        {
            DLOGT(LOG_BEGIN("PipeChannel(%s, %d, %d)"), pipe_name, mode, access);
            if (pipe_name != nullptr && *pipe_name != '\0')
            {
                std::strcpy(pipename_, pipe_name);
            }
            else
            {
                std::memset(pipename_, 0, sizeof(pipename_));
            }
        }

        PipeChannel::PipeChannel(PipeChannel&& channel) :
            handle_(channel.handle_),
            mode_(channel.mode_),
            access_(channel.access_)
        {
            DLOGT(LOG_BEGIN("PipeChannel(&%x)"), DLOG_POINTER(&channel));
            channel.handle_ = -1;
            channel.mode_ = 0;
            channel.access_ = 0;
            std::strcpy(pipename_, channel.pipename_);
            *channel.pipename_ = '\0';
        }

        PipeChannel::~PipeChannel()
        {
            DLOGT(LOG_BEGIN("~PipeChannel(): handle_ = %d"), handle_);
            if (handle_ >= 0)
            {
                Close();
            }
        }

        int PipeChannel::Open()
        {
            int rc = -EEXIST;

            DLOGT(LOG_BEGIN("Open()"));


            if (handle_ < 0)
            {

                rc = -EINVAL;

                if (pipename_ != '\0')
                {

                    rc = 0;

                    if (access_ != 0)
                    {
                        //::unlink(name);

                        DLOGT(LOG_BEGIN("Open(): create new pipe access_ = %d"), access_);

                        rc = ::mkfifo(pipename_, access_);

                        if (rc < 0 && errno != EEXIST)
                        {

                            rc = -errno;
                            DLOGW(LOG_BEGIN("Open(): pipe create error, rc = %d"), rc);

                        }
                        else
                        {
                            DLOGI(LOG_BEGIN("Open(): pipe create success, rc = %d"), rc);

                            rc = 0;
                        }
                    }

                    if (rc >= 0)
                    {
                        rc = ::open(pipename_, mode_ & ~O_CREAT);

                        if (rc >= 0)
                        {

                            handle_ = rc;

                            DLOGI(LOG_BEGIN("Open(): pipe open success, rc = %d"), rc);
                        }
                        else
                        {
                            rc = -errno;

                            DLOGE(LOG_BEGIN("Open(): pipe open error, rc = %d"), rc);
                        }
                    }

                }
                else
                {
                    DLOGW(LOG_BEGIN("Open(): invalid argument, name = %s"), pipename_);
                }
            }
            else
            {
                DLOGW(LOG_BEGIN("Open(): pipe already open, handle_ = %d"), handle_);
            }

            return rc;
        }

        int PipeChannel::Close()
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                rc = ::close(handle_);

                if (rc >= 0)
                {

                    rc = handle_;

                    handle_ = -1;

                    DLOGI(LOG_BEGIN("Close(): pipe close success, handle_ = %d"), rc);
                }
                else
                {
                    rc = -errno;
                    DLOGE(LOG_BEGIN("Close(): Error close pipe, handle_ = %d, rc = %d"), handle_, rc);
                }
            }
            else
            {
                DLOGD(LOG_BEGIN("Close(): pipe already close, handle_ = %d"), handle_);
            }

            return rc;
        }

        int PipeChannel::Write(const void* msg, int size, int flags)
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                rc = -EINVAL;

                if (msg != nullptr && size > 0)
                {

                    rc = ::write(handle_, msg, size);

                    if (rc < 0)
                    {
                        rc = -errno;
                        DLOGW(LOG_BEGIN("Write(%x, %d): write error rc = %d"), DLOG_POINTER(msg), size, rc);
                    }
                    else
                    {
                        DLOGT(LOG_BEGIN("Write(%x, %d): write success %d bytes"), DLOG_POINTER(msg), size, rc);
                    }

                }
                else
                {
                    DLOGW(LOG_BEGIN("Write(%x, %d): invalid arguments"), DLOG_POINTER(msg), size);
                }

            }
            else
            {
                DLOGW(LOG_BEGIN("Write(): pipe not open handle_ = %d"), handle_);
            }

            return rc;
        }

        int PipeChannel::Read(void *msg, int size, int flags)
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                rc = -EINVAL;

                if (msg != nullptr && size > 0)
                {
                    rc = ::read(handle_, msg, size);

                    if (rc <= 0)
                    {
                        rc = -errno;
                        DLOGW(LOG_BEGIN("Read(%x, %d): read error rc = %d"), DLOG_POINTER(msg), size, rc);
                    }
                    else
                    {
                        DLOGT(LOG_BEGIN("Read(%x, %d): read success %d bytes"), DLOG_POINTER(msg), size, rc);
                    }


                }
                else
                {
                    DLOGW(LOG_BEGIN("Read(%x, %d): invalid arguments"), DLOG_POINTER(msg), size);
                }

            }
            else
            {
                DLOGW(LOG_BEGIN("Read(): pipe not open handle_ = %d"), handle_);
            }

            return rc;
        }

        bool PipeChannel::IsCanWrite() const
        {
            return handle_ >= 0;
        }

        bool PipeChannel::IsCanRead() const
        {
            return handle_ >= 0;
        }

        int PipeChannel::Handle() const
        {
            return handle_;
        }

        const char* PipeChannel::Name() const
        {
            return pipename_;
        }


    }
}
