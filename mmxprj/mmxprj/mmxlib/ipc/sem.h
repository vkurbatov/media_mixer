#ifndef _MMX_IPC_SEMAPHORE__H
#define _MMX_IPC_SEMAPHORE_H

#define MMX_SEM_NAME_LEN   256

#include <memory>

namespace mmx
{
    namespace ipc
    {
        class Semaphore
        {
            int     handle_;
            int     entries_;

        public:
            Semaphore();
            //PipeChannel(const PipeChannel& channel);
            Semaphore(Semaphore&& semaphore);
            ~Semaphore();
            int Open(int key, ...);
            int Close();
            int Handle() const;
            int Set(int val = 1);
            int Get() const;
        };
    }
}


#endif
