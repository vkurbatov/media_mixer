#include "mmxlib/ipc/sem.h"
#include "mmxlib/ipc/shmem.h"

#include <cstring>
#include <iostream>

#include <fcntl.h>

#define SEMAPHORE_NAME  "/tmp/sem_new"
#define SHMEM_NAME  "shm_test-1"


namespace mmxtest
{

    int test10()
    {     

        mmx::ipc::Semaphore sem1, sem2;
        mmx::ipc::SharedMemory shmem1, shmem2;

        int rc = sem1.Open(1002);
        rc = sem2.Open(1002,0);

        rc = sem1.Set(1);
        rc = sem2.Set(1);

        rc = sem1.Set(-1);
        rc = sem2.Set(-1);

        rc = sem1.Get();

        rc = shmem1.Open(SHMEM_NAME, O_RDWR | O_CREAT, 10);
        rc = shmem2.Open(SHMEM_NAME, O_RDWR, 10);

        ((char*)shmem1.Data())[9] = 5;
        shmem1.Close();
        rc = ((char*)shmem2.Data())[9];

        std::cout << rc << std::endl;

    }
}
