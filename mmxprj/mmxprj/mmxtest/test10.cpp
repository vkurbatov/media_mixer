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

        mmx::ipc::Semaphore sem1(SHMEM_ID, 0666), sem2(SHMEM_ID,0);
        mmx::ipc::SharedMemory shmem1(SHMEM_ID, 10), shmem2(SHMEM_ID, 0);

        int rc = sem1.Open();
        rc = sem2.Open();

        rc = sem1.Set(1);
        rc = sem2.Set(1);

        rc = sem1.Set(-1);
        rc = sem2.Set(-1);

        rc = sem1.Get();

        rc = shmem1.Open();
        rc = shmem2.Open();

        void* data = shmem1.Data();

        shmem1.Close();
        ((char*)data)[10] = 5;
        rc = ((char*)shmem2.Data())[10];

        std::cout << rc << std::endl;

    }
}
