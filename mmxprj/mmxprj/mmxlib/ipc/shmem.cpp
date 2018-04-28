
#include "shmem.h"

#include <sys/shm.h>

#include <cstdarg>      // работа с переменнам количеством аргументов
#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("SharedMemory", msg)

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
            DLOGT(LOG_BEGIN("SharedMemory()"));
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

            DLOGT(LOG_BEGIN("SharedMemory(&&%d)"), DLOG_POINTER(&shmem));
        }

        SharedMemory::~SharedMemory()
        {
            DLOGT(LOG_BEGIN("~SharedMemory()"));
            Close();
        }

        int SharedMemory::Open(int key, int size, int mode)
        {
            int rc = -EEXIST;


            DLOGT(LOG_BEGIN("SharedMemory(%d, %d, %d)"), key, size, mode);

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

                            DLOGI(LOG_BEGIN("Open(%d, %d, %d): shemm open success, data_ = %x, size_ = %d,  rc = %d"), key, size, mode, DLOG_POINTER(data_), size_, rc);

                        }
                        else
                        {

                            rc = -errno;
                            shmctl(handle_, IPC_RMID, NULL);
                            handle_ = -1;
                            DLOGE(LOG_BEGIN("Open(%d, %d, %d): get shmem pointer error, rc = %d"), key, size, mode, rc);

                        }
                    }
                    else
                    {
                        rc =-errno;
                        DLOGE(LOG_BEGIN("Open(%d, %d, %d): open shmem error, rc = %d"), key, size, mode, rc);
                    }

                }
                else
                {
                    DLOGE(LOG_BEGIN("Open(%d, %d, %d): invalid argument"), key, size, mode);
                }

            }
            else
            {
                DLOGW(LOG_BEGIN("Open(): already open, handle_ = %d"), handle_);
            }


            return rc;

        }

        int SharedMemory::Close()
        {
            int rc =-EBADF;


            if (data_ != nullptr)
            {

                rc = shmdt(data_);

                if (rc >= 0)
                {
                    DLOGD(LOG_BEGIN("Close() release data = %x success)"), DLOG_POINTER(data_));
                }
                else
                {
                    rc = -errno;
                    DLOGD(LOG_BEGIN("Close() release data = %x error, rc = %d)"), DLOG_POINTER(data_), rc);
                }

                data_ = nullptr;
                size_ = 0;

            }

            if (handle_ >= 0)
            {

                rc = shmctl( handle_, IPC_RMID, NULL);

                if (rc >= 0)
                {
                    DLOGD(LOG_BEGIN("Close() close shmem %d success)"), handle_);
                }
                else
                {
                    rc = -errno;
                    DLOGD(LOG_BEGIN("Close() close shmem %d error, rc = %d)"), handle_, rc);
                }

                handle_ = -1;

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
