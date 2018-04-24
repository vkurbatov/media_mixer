
#include "shmem.h"

#include <sys/mman.h>

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
            std::memset(name_, 0, sizeof(name_));
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
            shmem.name_[0] = 0;
            std::strcpy(name_,shmem.name_);
            shmem.mode_ = 0;
        }

        SharedMemory::~SharedMemory()
        {
            Close();
        }

        int SharedMemory::Open(const char *name, ...)
        {
            int rc = -EEXIST;

            std::va_list    vl;

            va_start (vl, name);

            int mode = va_arg(vl, int);
            int size = va_arg(vl, int);


            if (handle_ < 0)
            {

                rc = shm_open(name, mode, 0666);

                if (rc > 0)
                {
                    if ((mode & O_CREAT) != 0)
                    {
                        if (ftruncate(rc, size + 1) >= 0)
                        {

                        }
                        else
                        {
                            close(rc);
                            shm_unlink(name);
                            rc = -errno;
                        }
                    }

                    if (rc > 0)
                    {
                        void* m = mmap(0, size + 1, PROT_WRITE | PROT_READ, MAP_SHARED, rc, 0);

                        if (m != nullptr)
                        {
                            data_ = m;
                            size_ = size;
                            mode_ = mode;
                            handle_ = rc;
                            std::strcpy(name_, name);
                        }
                    }
                }
                else
                {
                    rc = -errno;
                }

            }

            va_end (vl);

            return rc;

        }

        int SharedMemory::Close()
        {
            int rc =-EBADF;


            if (handle_ >= 0)
            {
                rc = handle_;

                if (data_ != nullptr)
                {
                    munmap(data_, size_);
                }

                close(handle_);

                if (mode_ & O_CREAT)
                {
                    shm_unlink(name_);
                }

                name_[0] = 0;
                data_ = nullptr;
                size_ = 0;
                handle_ = -1;
            }

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
