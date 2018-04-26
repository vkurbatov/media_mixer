#ifndef _MMX_IPC_SHMEM__H
#define _MMX_IPC_SHMEM_H

#define MMX_SHMEM_NAME_LEN   256

#include <memory>

namespace mmx
{
    namespace ipc
    {
        class SharedMemory
        {
            int     handle_;
            int     size_;
            void*   data_;
            int     mode_;

        public:
            SharedMemory();
            SharedMemory(SharedMemory&& semaphore);
            ~SharedMemory();
            int Open(int key, int size = 0, int mode = 0);
            int Close();
            int Handle() const;
            void* Data();
            int Size();
        };
    }
}


#endif
