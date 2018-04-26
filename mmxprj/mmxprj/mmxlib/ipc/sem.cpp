#include "sem.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <cstring>
#include <cstdarg>      // работа с переменнам количеством аргументов

#include <errno.h>

namespace mmx
{
    namespace ipc
    {
        Semaphore::Semaphore() :
            handle_(-1),
            entries_(0)
        {

        }

        Semaphore::Semaphore(Semaphore&& semaphore):
            handle_(semaphore.handle_),
            entries_(semaphore.entries_)
        {

        }

        Semaphore::~Semaphore()
        {
            Close();
        }

        int Semaphore::Open(int key, ...)
        {
            int rc = -EEXIST;

            std::va_list     vl;

            va_start (vl, key);

            int mode = va_arg(vl, int) & 0x1FF;

            if (handle_ < 0)
            {

                if (key > 0)
                {

                    if (mode != 0)
                    {
                        rc = semget(key, 1, mode | IPC_CREAT);
                    }
                    else
                    {
                        rc = semget(key, 1, IPC_EXCL);
                    }

                    if (rc >= 0)
                    {
                        handle_ = rc;
                    }
                    else
                    {
                        rc = -errno;
                    }
                }
                else
                {
                    rc = -errno;
                }
            }

            return rc;

        }

        int Semaphore::Set(int val)
        {
            int rc = -EBADF;

            if (handle_ >= 0)
            {

                entries_ += val;

                struct sembuf s_buff;

                s_buff.sem_num = 0;
                s_buff.sem_flg = SEM_UNDO;
                s_buff.sem_op = val;

                rc = semop(handle_, &s_buff, 1);

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
                }

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
