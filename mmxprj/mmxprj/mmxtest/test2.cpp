#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <memory>

#include "defines.h"

#include "mmxlib/net/socket.h"
#include "mmxlib/net/select.h"
#include "mmxlib/ipc/pchannel.h"

#include "mmxlib/headers/media.h"
#include "mmxlib/headers/udp.h"

#include "mmxlib/logs/dlog.h"

#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

#define CHANNEL_NAME "/tmp/testpipe01"

#include "mmxlib/net/select_ex.h"

namespace mmxtest
{
    
    int test2()
    {

        char w_buff[100];
        char r_buff[100] = { 0 };

        std::sprintf(w_buff,"IP_Address: " DLOG_IP_FMT " nice",DLOG_IP(0x7F000001));

        mmx::ipc::PipeChannel w_channel(CHANNEL_NAME, O_RDWR, 0777);

        mmx::ipc::PipeChannel r_channel1(CHANNEL_NAME, O_RDONLY);
        mmx::ipc::PipeChannel r_channel2(CHANNEL_NAME, O_RDONLY);

        w_channel.Open();
        r_channel1.Open();
        r_channel2.Open();


        int rc = w_channel.Write(w_buff, strlen(w_buff) + 1);

        if (rc > 0)
        {
            std::cout << "Channel write " << rc << " bytes" << std::endl;

            rc = r_channel1.Read(r_buff, sizeof(r_buff));

            if (rc > 0)
            {
                std::cout << "Channel #1 read " << rc << " bytes:" << std::endl << r_buff << std::endl ;
            }
            else
            {
                std::cout << "Channel #1 read error " << rc << std::endl;
            }

            rc = r_channel2.Read(r_buff, sizeof(r_buff));

            if (rc > 0)
            {
                std::cout << "Channel #2 read " << rc << " bytes:" << std::endl << r_buff << std::endl ;
            }
            else
            {
                std::cout << "Channel #2 read error " << rc << std::endl;
            }
        }
        else
        {
            std::cout << "Channel write error " << rc << std::endl;
        }

        std::cout.flush();

        w_channel.Close();
        r_channel1.Close();
        r_channel2.Close();

    }

    int test2_1()
    {

        char w_buff[] = "Kurbatov";
        char r_buff[100] = { 0 };

        mmx::ipc::PipeChannel w_channel(CHANNEL_NAME, O_RDONLY | O_NONBLOCK, 0777);

        mmx::ipc::PipeChannel r_channel(CHANNEL_NAME, O_WRONLY | O_NONBLOCK);

        mmx::net::SelectExtension sel;


        int rc = 0;

        rc = r_channel.Open();


        rc = w_channel.Open();

        sel.Set(rc, mmx::net::S_EV_READ);

        rc = w_channel.Write(w_buff, sizeof(w_buff));

        r_channel.Close();

        rc = sel.Wait();

        if (rc > 0)
        {
            std::cout << "Channel write " << rc << " bytes" << std::endl;

            rc = r_channel.Read(r_buff, sizeof(r_buff));

            if (rc > 0)
            {
                std::cout << "Channel 1 read " << rc << " bytes:" << std::endl << r_buff << std::endl ;
            }
            else
            {
                std::cout << "Channel 1 read error " << rc << std::endl;
            }
        }
        else
        {
            std::cout << "Channel write error " << rc << std::endl;
        }

        std::cout.flush();

        w_channel.Close();
        r_channel.Close();

    }

}
