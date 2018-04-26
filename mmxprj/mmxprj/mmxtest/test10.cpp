#include "mmxlib/ipc/sem.h"
#include "mmxlib/ipc/shmem.h"

#include <cstring>
#include <iostream>

#include <fcntl.h>

#define SEMAPHORE_NAME  "/tmp/sem_new"
#define SHMEM_ID        1011L


namespace mmxtest
{

    int test10()
    {     

        mmx::ipc::Semaphore sem1, sem2;
        mmx::ipc::SharedMemory shmem1, shmem2;

        int rc = sem1.Open(SHMEM_ID, 0666);
        rc = sem2.Open(SHMEM_ID,0);

        rc = sem1.Set(1);
        rc = sem2.Set(1);

        rc = sem1.Set(-1);
        rc = sem2.Set(-1);

        rc = sem1.Get();

        rc = shmem1.Open(SHMEM_ID, 10, 0666);
        rc = shmem2.Open(SHMEM_ID, 0);

        void* data = shmem1.Data();

        shmem1.Close();
        ((char*)data)[10] = 5;
        rc = ((char*)shmem2.Data())[10];

        std::cout << rc << std::endl;

    }
}
