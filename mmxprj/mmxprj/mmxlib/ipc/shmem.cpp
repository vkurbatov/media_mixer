
#include "shmem.h"

#include <sys/shm.h>

#include <cstdarg>      // работа с переменнам количеством аргументов
#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


namespace mmx
{
    namespace ipc
    {
        SharedMemory::SharedMemory() :
            handle_(-1),
            size_(0),
            data_(nullptr),
            mode_(0)
        {

        }

        SharedMemory::SharedMemory(SharedMemory&& shmem) :
            handle_(shmem.handle_),
            size_(shmem.size_),
            data_(shmem.data_),
            mode_(shmem.mode_)
        {
            shmem.handle_ = -1;
            shmem.size_ = 0;
            shmem.data_ = nullptr;
            shmem.mode_ = 0;
        }

        SharedMemory::~SharedMemory()
        {
            Close();
        }

        int SharedMemory::Open(int key, int size, int mode)
        {
            int rc = -EEXIST;

            std::va_list    vl;


            if (handle_ < 0)
            {
                //shmget()

                rc = -EINVAL;

                if (size > 0 || (size == 0 & mode == 0))
                {

                    rc = shmget(key, size, mode != 0 ? mode | IPC_CREAT : IPC_EXCL);

                    if (rc >= 0)
                    {
                        handle_ = rc;

                        data_ = shmat(rc, NULL, 0);

                        if (data_ != nullptr)
                        {
                            struct shmid_ds ds;

                            shmctl(handle_, IPC_STAT, &ds);
                            size_ = ds.shm_segsz;

                            mode_ = mode;

                        }
                        else
                        {

                            rc = -errno;
                            shmctl(handle_, IPC_RMID, NULL);
                            handle_ = -1;

                        }
                    }
                    else
                    {
                        rc =-errno;
                    }

                }

            }

            va_end (vl);

            return rc;

        }

        int SharedMemory::Close()
        {
            int rc =-EBADF;


            if (data_ != nullptr)
            {

                rc = shmdt(data_);
                data_ = nullptr;
                size_ = 0;

            }

            if (handle_ >= 0)
            {

                rc = shmctl( handle_, IPC_RMID, NULL);
                handle_ = 0;

            }

            mode_ = 0;

            return rc;
        }

        int SharedMemory::Handle() const
        {
            return handle_;
        }

        void* SharedMemory::Data()
        {
            return data_;
        }

        int SharedMemory::Size()
        {
            return size_;
        }

    }
}
