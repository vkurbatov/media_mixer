#ifndef _MMX_IPC_SEMAPHORE__H
#define _MMX_IPC_SEMAPHORE_H

#define MMX_SEM_NAME_LEN   256

#include "ichannel.h"

#include <memory>

namespace mmx
{
    namespace ipc
    {
        class Semaphore : public IChannel
        {
            int     handle_;
            int     key_;
            int     mode_;
            int     entries_;

        public:
            Semaphore(int key, int mode = 0);
            //PipeChannel(const PipeChannel& channel);
            Semaphore(Semaphore&& semaphore);
            ~Semaphore() override;

            // IChannel interface
            int Open() override;
            int Close() override;
            int Handle() const override;


            int Set(int val = 1);
            int Get() const;

        };
    }
}


#endif
