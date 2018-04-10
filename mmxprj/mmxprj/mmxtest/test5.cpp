#include "defines.h"
#include <iostream>

#include <cstring>

#include "mmxlib/net/socket.h"
#include "mmxlib/tools/iosockadp.h"
#include "mmxlib/tools/deffwriter.h"

#include <netdb.h>
#include <fcntl.h>

#define SEND_BUFFER_SIZE 1024
#define TEST_DATA_SIZE 16

namespace mmxtest
{

    int print_dump(const void* dump, int size)
    {

        const char* p = (const char*)dump;
        for (int i = 0; i < size; i++)
        {
            if (i > 0)
            {
                 std::cout << ", ";
            }

            std::cout << (int)p[i];

        }

    }

    int test5()
    {

        unsigned char w_buff[SEND_BUFFER_SIZE];
        unsigned char r_buff[sizeof(w_buff)];

        for (int i = 0; i < sizeof(w_buff); i++)
        {
            w_buff[i] = (unsigned char)i;
        }

        mmx::net::address_t address = mmx::net::Socket::StoA("127.0.0.1");
        mmx::net::port_t port = 6000;
        mmx::net::Socket udp(SOCK_DGRAM, IPPROTO_UDP);

        mmx::tools::SocketIOAdapter sockadp(udp, &address, &port);

        mmx::tools::DeferredWriter deffwriter(sockadp, 2);

        int rc = udp.Open(address, port, 0, 0, 0, O_NONBLOCK);

        int w_pos = 0;

        while (rc >= 0 && w_pos < SEND_BUFFER_SIZE)
        {

            rc = deffwriter.Write(w_buff + w_pos, 8, 0);

            w_pos += 8;

            if (rc >= 0)
            {
                std::cout << "Udp write complete " << rc << " bytes." << std::endl;

                //print_dump(w_buff, rc);
                std::cout << std::endl;


                rc = deffwriter.Read(r_buff, sizeof(r_buff));

                if (rc >= 0)
                {

                    std::cout << "Udp read complete " << rc << " bytes." << std::endl;

                    print_dump(r_buff, rc);
                    std::cout << std::endl;


                }
                else
                {
                    std::cout << "Udp read error witn rc = " << rc << "." << std::endl;
                }

                std::cout << "deffwriter status: " << std::endl;

                std::cout << "Count: " << deffwriter.Count() << std::endl;
                std::cout << "Last Result: " << deffwriter.LastResult() << std::endl;
                std::cout << "Lost Data: " << deffwriter.Lost() << std::endl;

            }
            else
            {
                std::cout << "Udp write error witn rc = " << rc << "." << std::endl;
            }



        }

        return rc;

    }
}
