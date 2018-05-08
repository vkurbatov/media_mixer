#ifndef _MMX_IPC_SHMEM__H
#define _MMX_IPC_SHMEM_H

#include "ichannel.h"

#include <memory>

namespace mmx
{
    namespace ipc
    {
        class SharedMemory : public IChannel
        {
            int     handle_;
            int     key_;
            int     size_;
            void*   data_;
            int     rsize_;

        public:
            SharedMemory(int key, int size = 0);
            SharedMemory(SharedMemory&& semaphore);
            ~SharedMemory();

            // IChannel interface
            int Open() override;
            int Close() override;
            int Handle() const override;

            void* Data();
            int Size();


        };
    }
}


#endif
