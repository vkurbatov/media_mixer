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

        mmx::ipc::PipeChannel r_channel(CHANNEL_NAME, O_RDONLY | O_NONBLOCK, 0777);

        mmx::ipc::PipeChannel w_channel1(CHANNEL_NAME, O_WRONLY | O_NONBLOCK);
        mmx::ipc::PipeChannel w_channel2(CHANNEL_NAME, O_WRONLY | O_NONBLOCK);

        mmx::net::SelectExtension sel;


        int rc = 0;

        rc = r_channel.Open();

        rc = w_channel1.Open();
        rc = w_channel2.Open();

        sel.Set(r_channel.Handle(), mmx::net::S_EV_READ);
        sel.Set(w_channel1.Handle(), mmx::net::S_EV_WRITE);
        sel.Set(w_channel2.Handle(), mmx::net::S_EV_WRITE);

        int i = 0;

        while ((rc = sel.Wait()) >= 0)
        {

            if (sel.IsRead(r_channel.Handle()))
            {
                rc = r_channel.Read(r_buff, sizeof(r_buff));

                if (rc > 0)
                {
                    std::cout << "[" << i << "] Input channel read " << rc << " bytes:" << std::endl << r_buff << std::endl ;
                }
                else
                {
                    std::cout << "[" << i << "] Input channel read error " << rc << std::endl;
                }
            }

            if (i < 50)
            {
                if (sel.IsWrite(w_channel1.Handle()))
                {
                    rc = w_channel1.Write(w_buff, sizeof(w_buff));

                    if (rc > 0)
                    {
                        std::cout << "[" << i << "] Ouput channel 1 write " << rc << " bytes:" << std::endl << w_buff << std::endl ;
                    }
                    else
                    {
                        std::cout << "[" << i << "] Ouput channel 1 write error " << rc << std::endl;
                    }
                }


                if (sel.IsWrite(w_channel2.Handle()))
                {
                    rc = w_channel2.Write(w_buff, sizeof(w_buff));

                    if (rc > 0)
                    {
                        std::cout << "[" << i << "] Ouput channel 2 write " << rc << " bytes:" << std::endl << w_buff << std::endl ;
                    }
                    else
                    {
                        std::cout << "[" << i << "] Ouput channel 2 write error " << rc << std::endl;
                    }
                }
            }
            else
            {
                if (i == 50)
                {
                    sel.Set(w_channel2.Handle());
                    w_channel2.Close();
                }
            }


            i++;

            if (i > 100)
                break;

        }

        std::cout.flush();

        r_channel.Close();        
        w_channel1.Close();
        w_channel2.Close();

    }

}
