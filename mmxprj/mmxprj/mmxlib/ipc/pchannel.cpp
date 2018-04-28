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
        PipeChannel::PipeChannel() :
            handle_(-1),
            mode_(0),
            access_(0)
        {
            DLOGT(LOG_BEGIN("PipeChannel()"));
            std::memset(pipename_, 0, sizeof(pipename_));
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

        int PipeChannel::Open(const char* name, ...)
        {
            int rc = -EEXIST;

            std::va_list     vl;

            va_start (vl, name);

            int mode = va_arg(vl, int);

            DLOGT(LOG_BEGIN("Open(%s, %d)"), name, mode);

            if (handle_ < 0)
            {
                rc = -EINVAL;

                if (name != nullptr && *name != '\0' && std::strlen(name) < MMX_PIPE_NAME_LEN)
                {

                    rc = 0;

                    if ((mode & O_CREAT) != 0)
                    {

                        mode ^= O_CREAT;

                        int access = va_arg(vl, int);
                        //::unlink(name);

                        DLOGT(LOG_BEGIN("Open(): create new pipe access = %d"), access);

                        rc = ::mkfifo(name, access);

                        if (rc < 0 && errno != EEXIST)
                        {

                            rc = -errno;
                            DLOGW(LOG_BEGIN("Open(): pipe create error, rc = %d"), rc);

                        }
                        else
                        {
                            DLOGI(LOG_BEGIN("Open(): pipe create success, rc = %d"), rc);

                            access_ = access;

                            rc = 0;
                        }
                    }

                    if (rc >= 0)
                    {
                        rc = ::open(name, mode);

                        if (rc >= 0)
                        {
                            mode_ = mode;

                            std::strcpy(pipename_,name);

                            handle_ = rc;

                            DLOGI(LOG_BEGIN("Open(): pipe open success, rc = %d"), rc);
                        }
                        else
                        {
                            rc = -errno;

                            if (access_ != 0)
                            {
                                //::unlink(name);
                                access_ = 0;
                            }

                            DLOGE(LOG_BEGIN("Open(): pipe open error, rc = %d"), rc);
                        }
                    }

                }
                else
                {
                    DLOGW(LOG_BEGIN("Open(): invalid argument, name = %s"), name);
                }
            }
            else
            {
                DLOGW(LOG_BEGIN("Open(): pipe already open, handle_ = %d"), handle_);
            }

            va_end(vl);

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

                    mode_ = 0;

                    if (access_ != 0)
                    {
                        //::unlink(pipename_);
                        access_ = 0;
                    }

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

        int PipeChannel::Handle() const
        {
            return handle_;
        }

        const char* PipeChannel::Name() const
        {
            return handle_ < 0 ? nullptr : pipename_;
        }

    }
}
