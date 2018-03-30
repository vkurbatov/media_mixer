#include "pchannel.h"

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>      // remove
#include <fcntl.h>      // open
#include <unistd.h>     // close

#include <cstring>      // strcpy

namespace mmx
{
    namespace ipc
    {
        PipeChannel::PipeChannel() :
            handle_(-1),
            mode_(0),
            access_(0),
            pipename_("\0")
        {

        }

        PipeChannel::PipeChannel(PipeChannel&& channel) :
            handle_(channel.handle_),
            mode_(channel.mode_),
            access_(channel.access_)
        {
            channel.handle_ = -1;
            channel.mode_ = 0;
            channel.access_ = 0;
            std::strcpy(pipename_, channel.pipename_);
            *channel.pipename_ = '\0';
        }

        PipeChannel::~PipeChannel()
        {
            if (handle_ >= 0)
            {
                Close();
            }
        }

        int PipeChannel::Open(const char* name, int mode, int access)
        {
            int rc = -EEXIST;

            if (handle_ < 0)
            {
                rc = -EINVAL;

                if (name != nullptr && *name != '\0' && std::strlen(name) < MMX_PIPE_NAME_LEN)
                {
                    rc = 0;

                    if (access != 0)
                    {

                        //::unlink(name);

                        rc = ::mkfifo(name, access);

                        if (rc < 0 && errno != EEXIST)
                        {
                            rc = -errno;
                        }
                        else
                        {
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
                        }
                        else
                        {
                            rc = -errno;

                            if (access_ != 0)
                            {
                                ::unlink(name);
                                access_ = 0;
                            }
                        }
                    }

                }
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

                    mode_ = 0;

                    if (access_ != 0)
                    {
                        ::unlink(pipename_);
                        access_ = 0;
                    }
                }
                else
                {
                    rc = -errno;
                }
            }

            return rc;
        }

        int PipeChannel::Write(const char* msg, int size, int flags)
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
                    }

                }

            }

            return rc;
        }

        int PipeChannel::Read(char*msg, int size, int flags)
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                rc = -EINVAL;

                if (msg != nullptr && size > 0)
                {

                    rc = ::read(handle_, msg, size);

                    if (rc < 0)
                    {
                        rc = -errno;
                    }

                }

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

        int PipeChannel::Mode() const
        {
            return mode_;
        }

        int PipeChannel::Access() const
        {
            return access_;
        }
    }
}
