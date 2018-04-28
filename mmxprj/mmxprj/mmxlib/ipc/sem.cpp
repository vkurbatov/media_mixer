#include "sem.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <cstring>
#include <cstdarg>      // работа с переменнам количеством аргументов

#include <errno.h>

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("Semaphore", msg)

namespace mmx
{
    namespace ipc
    {
        Semaphore::Semaphore() :
            handle_(-1),
            entries_(0)
        {
            DLOGT(LOG_BEGIN("Semaphore()"));
        }

        Semaphore::Semaphore(Semaphore&& semaphore):
            handle_(semaphore.handle_),
            entries_(semaphore.entries_)
        {
            DLOGT(LOG_BEGIN("Semaphore(&&%x)"), DLOG_POINTER(&semaphore));
        }

        Semaphore::~Semaphore()
        {
            DLOGT(LOG_BEGIN("Semaphore()"));
            Close();
        }

        int Semaphore::Open(int key, ...)
        {
            int rc = -EEXIST;

            std::va_list     vl;

            va_start (vl, key);

            int mode = va_arg(vl, int) & 0x1FF;

            DLOGT(LOG_BEGIN("Open(%d, %d)"), key, mode);

            if (handle_ < 0)
            {

                rc = -EINVAL;

                if (key > 0)
                {

                    if (mode != 0)
                    {
                        rc = semget(key, 1, mode | IPC_CREAT);
                        DLOGD(LOG_BEGIN("Open(): create semaphore, rc = %d"), rc);
                    }
                    else
                    {
                        rc = semget(key, 1, IPC_EXCL);
                        DLOGD(LOG_BEGIN("Open(): open semaphore, rc = %d"), rc);
                    }

                    if (rc >= 0)
                    {
                        handle_ = rc;
                        DLOGD(LOG_BEGIN("Open(): open semaphore success, rc = %d"), rc);
                    }
                    else
                    {
                        rc = -errno;
                        DLOGD(LOG_BEGIN("Open(): open semaphore error, rc = %d"), rc);
                    }
                }
                else
                {
                    rc = -errno;
                    DLOGW(LOG_BEGIN("Open(): invalid argument, key = %d"), key);
                }
            }
            else
            {
                DLOGW(LOG_BEGIN("Open(): semaphore already open, handle_ = %d"), handle_);
            }

            return rc;

        }

        int Semaphore::Set(int val)
        {
            int rc = -EBADF;


            DLOGT(LOG_BEGIN("Set(%d)"), val);

            if (handle_ >= 0)
            {

                struct sembuf s_buff;

                s_buff.sem_num = 0;
                s_buff.sem_flg = SEM_UNDO;
                s_buff.sem_op = val;

                rc = semop(handle_, &s_buff, 1);

                if (rc >= 0)
                {
                    entries_ += val;
                    DLOGD(LOG_BEGIN("Set(%d) success!"), val);
                }
                else
                {
                    rc = -errno;
                    DLOGE(LOG_BEGIN("Set(%d) error, rc = %d!"), val, rc);
                }
            }
            else
            {
                DLOGW(LOG_BEGIN("Set(): semaphore not open, handle_ = %d"), handle_);
            }

            return rc;
        }

        int Semaphore::Get() const
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {

                rc = semctl(handle_, 0, GETVAL, 0 );

                if (rc < 0)
                {
                    rc = -errno;
                    DLOGE(LOG_BEGIN("Get() Error, rc = %d!"), rc);
                }
                else
                {
                    DLOGD(LOG_BEGIN("Get() Success, rc = %d!"), rc);
                }

            }
            else
            {
                DLOGW(LOG_BEGIN("Get(): semaphore not open, handle_ = %d"), handle_);
            }


            return rc;
        }

        int Semaphore::Close()
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {
                if (entries_ > 0)
                {
                    Set(-entries_);
                }

                DLOGD(LOG_BEGIN("Close() Success, handle_ = %d!"), handle_);
                handle_ = -1;
            }

            return rc;

        }

        int Semaphore::Handle() const
        {
            return handle_;
        }

    }

}
