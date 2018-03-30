#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <memory>

#include "defines.h"

#include "mmxlib/net/socket.h"
#include "mmxlib/net/select.h"

#include "mmxlib/headers/media.h"
#include "mmxlib/headers/udp.h"

#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>


namespace mmxtest
{

    static int g_send_packs = 0;
    static int g_send_bytes = 0;
    static int g_recv_packs = 0;
    static int g_recv_bytes = 0;
    static int g_sync_err_count = 0;
    static int g_max_sync_err_window = 0;
    static int g_first_rcv_id = 0;
    static int g_last_rcv_id = 0;

    int udp_thread()
    {


        mmx::net::Socket udp(SOCK_RAW, IPPROTO_UDP);
        char r_buff[1600] = { 0 };

        mmx::headers::UDPPACKET &udp_pack = *(mmx::headers::PUDPPACKET)r_buff;
        mmx::headers::MEDIA_DATA &media = *(mmx::headers::PMEDIA_DATA)udp_pack.data;

        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        //int rc = udp.Open(mmx::net::Socket::StoA("127.0.0.1"), 6000);

        int rc = udp.Open();

        mmx::net::address_t addr = 0;
        mmx::net::port_t port = 0;

        mmx::net::Select sel;

        sel.Set(udp.Handle(), mmx::net::S_EV_READ);

        int cnt = 0;


        while (rc > 0)
        {

            rc = sel.Wait(1000);

            if (rc >= 0)
            {


                rc = udp.Recv(r_buff, sizeof(udp_pack.ip4_header) +  sizeof(udp_pack.udp_header) + sizeof(media.header) + 1400, MSG_WAITALL);

                if (rc >= 0)
                {
                    if (media.header.magic == mmx::headers::MEDIA_MAGIC)
                    {
                        //rc = tcp.Recv(media.media, media.header.length, MSG_WAITALL);

                        int& cur = *(int*)media.media;

                        g_last_rcv_id = cur;

                        if (g_first_rcv_id == 0)
                            g_first_rcv_id = cur;

                        //if (rc == media.header.length)
                        {
                            //std::cout << *(int*)media.media << std::endl;

                             //std::cout << "error" << std::endl;

                            g_recv_packs ++;
                            g_recv_bytes += rc;

                            int diff = cur - cnt;

                            if (diff > g_max_sync_err_window)
                            {
                                g_max_sync_err_window = cur - cnt;
                            }


                            g_sync_err_count += (int)(diff > 1);

                            cnt = cur;

                        }
                        /*else
                        {
                            std::cout << "error size = " << rc << std::endl;
                        }*/

                    }
                    else
                    {
                        std::cout << "error magic = " << media.header.magic << std::endl;
                    }
                    //std::cout << rc << std::endl;
                }
            }
        }

        udp.Close();

        return rc;
    }

    int client_thread()
    {


        mmx::net::Socket tcp(SOCK_STREAM);
        char r_buff[1600] = { 0 };
        mmx::headers::MEDIA_DATA &media = *(mmx::headers::PMEDIA_DATA)r_buff;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int rc = tcp.Open(0, 0, mmx::net::Socket::StoA("127.0.0.1"), 6000);

        mmx::net::address_t addr = 0;
        mmx::net::port_t port = 0;

        mmx::net::Select sel;

        sel.Set(tcp.Handle(), mmx::net::S_EV_READ);

        int cnt = 0;


        while (rc>0)
        {

            rc = sel.Wait(1000);

            if (rc >= 0)
            {


                rc = tcp.Recv((char*)&media, sizeof(media.header) + 1400, MSG_WAITALL);

                if (rc >= 0)
                {


                    if (media.header.magic == mmx::headers::MEDIA_MAGIC)
                    {
                        //rc = tcp.Recv(media.media, media.header.length, MSG_WAITALL);

                        int& cur = *(int*)media.media;

                        //if (rc == media.header.length)
                        {
                            //std::cout << *(int*)media.media << std::endl;

                             //std::cout << "error" << std::endl;

                            g_recv_packs ++;
                            g_recv_bytes += rc;

                            int diff = cur - cnt;

                            if (diff > g_max_sync_err_window)
                            {
                                g_max_sync_err_window = cur - cnt;
                            }


                            g_sync_err_count += (int)(diff > 1);

                            cnt = cur;

                        }
                        /*else
                        {
                            std::cout << "error size = " << rc << std::endl;
                        }*/

                    }
                    else
                    {
                        std::cout << "error magic = " << media.header.magic << std::endl;
                    }
                    //std::cout << rc << std::endl;
                }
            }
        }

        tcp.Close();

        return rc;
    }

    int test1()
    {
        mmx::net::Socket udp(SOCK_DGRAM);
        //mmx::net::Socket tcp(SOCK_STREAM);

        char w_buff[1400];

        int rc = udp.Open();

        for (int i = 0; i < sizeof(w_buff); i++)
        {
            w_buff[i] = (char)(i);
        }

        auto start = std::chrono::system_clock::now();


        if (rc >= 0)
        {
            //rc = tcp.Open(0, 0, mmx::net::Socket::StoA("127.0.0.1"), 6000);

            if (rc >= 0)
            {

                bool f_test = true;

                std::thread th(client_thread);

                auto send_addr = mmx::net::Socket::StoA("127.0.0.1");

                while (f_test)
                {
                    auto end = std::chrono::system_clock::now();


                    f_test = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 1;

                    (*(int*)w_buff) = g_send_packs;

                    rc = udp.Send(w_buff, sizeof(w_buff), 0, send_addr, 5000);

                    if (rc >= 0)
                    {

                        g_send_packs++;
                        g_send_bytes += rc;

                        // std::cout << "Udp send message " << rc << " bytes!" << std::endl;

                        /*mmx::net::address_t addr = 0;
                        mmx::net::port_t port = 0;

                        rc = tcp.Recv(r_buff, sizeof(r_buff), 0, &addr, &port);

                        if (rc > 0)
                        {
                            auto delay =  std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - end).count();

                            avg_delay = avg_delay;

                            if (delay > max_delay)
                                max_delay = delay;

                            //std::cout << "Tcp recv message " << rc << " bytes!" << std::endl;

                            if (media.header.length == sizeof(w_buff))
                            {
                               // if (std::memcmp(w_buff, media.media, sizeof(w_buff)) == 0)
                                {
                                    iter++;

                                    f_test = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 1;
                                }

                            }

                        }
                        else
                        {
                            f_test = false;
                            std::cout << "Error tcp recv, rc = " << rc << std::endl;
                        }*/
                    }
                    else
                    {
                         std::cout << "Error udp send, rc = " << rc << std::endl;
                    }
                }//while

                th.join();

                //tcp.Close();

            }
            /*
            else
            {
                std::cout << "Error tcp open, rc = " << rc << std::endl;
            }*/

            udp.Close();
        }
        else
        {
            std::cout << "Error udp open, rc = " << rc << std::endl;
        }

        std::cout << "Statistic of test: " << std::endl;
        std::cout << "Packets send = " << g_send_packs << " in " << g_send_bytes << " bytes." << std::endl;
        std::cout << "Packets recv = " << g_recv_packs << " in " << g_recv_bytes << " bytes." << std::endl;
        std::cout << "Sync errors = " << g_sync_err_count << std::endl;
        std::cout << "Max sync error window = " << g_max_sync_err_window << std::endl;
    }

    int test1_1()
    {
        mmx::net::Socket tcp(SOCK_STREAM);

        char w_buff[1600] = { 0 };

        mmx::headers::MEDIA_DATA &media = *(mmx::headers::PMEDIA_DATA)w_buff;

        auto srv_addr = mmx::net::Socket::StoA("127.0.0.1");

        int rc = tcp.Open(srv_addr, 6000, 0, 0, 4);

        std::thread th(client_thread);

        mmx::net::Socket client(tcp);

        if (client.Handle() >= 0)
        {
            media.header.magic = mmx::headers::MEDIA_MAGIC;
            media.header.addr_dst = client.RemoteAddress();
            media.header.addr_src = tcp.LocalAddress();
            media.header.port_dst = client.RemoteAddress();
            media.header.port_src = tcp.LocalPort();
            media.header.length = 1400;

            auto start = std::chrono::system_clock::now();

            bool f_test = true;

            while (f_test)
            {
                auto end = std::chrono::system_clock::now();
                f_test = false;

                rc = client.Send(w_buff, media.header.length + sizeof(media.header));

                if (rc > 0)
                {
                    g_send_packs ++;
                    g_send_bytes += rc;

                    f_test  = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 1;
                }

            }

        }

        th.join();

        std::cout << "Statistic of test: " << std::endl;
        std::cout << "Packets send = " << g_send_packs << " in " << g_send_bytes << " bytes." << std::endl;
        std::cout << "Packets recv = " << g_recv_packs << " in " << g_recv_bytes << " bytes." << std::endl;
        std::cout << "Sync errors = " << g_sync_err_count << std::endl;
        std::cout << "Max sync error window = " << g_max_sync_err_window << std::endl;

        return rc;
    }

    int test1_2()
    {
        mmx::net::Socket udp(SOCK_DGRAM, IPPROTO_UDP);

        char w_buff[1600] = { 0 };

        mmx::headers::MEDIA_DATA &media = *(mmx::headers::PMEDIA_DATA)w_buff;

        auto srv_addr = mmx::net::Socket::StoA("127.0.0.1");

        int rc = udp.Open(0,0,0,0,0, O_NONBLOCK);

        mmx::net::Select sel;

        std::thread th(udp_thread);

        if (udp.Handle() >= 0)
        {
            media.header.magic = mmx::headers::MEDIA_MAGIC;
            media.header.addr_dst = srv_addr;
            media.header.addr_src = 0;
            media.header.port_dst = 6000;
            media.header.port_src = 0;
            media.header.length = 1400;

            auto start = std::chrono::system_clock::now();

            bool f_test = true;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            sel.Set(udp.Handle(), mmx::net::S_EV_WRITE);

            while (f_test)
            {
                auto end = std::chrono::system_clock::now();
                f_test = false;

                *(int*)media.media = g_send_packs;

                sel.Wait();

                rc = udp.Send(w_buff, media.header.length + sizeof(media.header), 0, srv_addr, 6000);

                if (rc > 0)
                {
                    g_send_packs ++;
                    g_send_bytes += rc;

                    f_test  = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 1;
                }
                else
                {
                    std::cout << "Send error rc = " << rc << std::endl;
                }

            }

        }

        th.join();

        std::cout << "Statistic of test: " << std::endl;
        std::cout << "Packets send = " << g_send_packs << " in " << g_send_bytes << " bytes." << std::endl;
        std::cout << "Packets recv = " << g_recv_packs << " in " << g_recv_bytes << " bytes." << std::endl;
        std::cout << "Sync errors = " << g_sync_err_count << std::endl;
        std::cout << "Max sync error window = " << g_max_sync_err_window << std::endl;
        std::cout << "g_first_rcv_id = " << g_first_rcv_id << std::endl;
        std::cout << "g_last_rcv_id = " << g_last_rcv_id << std::endl;
        std::cout << "lost: " << 100.0f - ((double)g_recv_packs / (double)g_send_packs ) * 100.0f << "% packets" << std::endl;
        //std::cout << " = " << g_max_sync_err_window << std::endl;

        return rc;
    }
}
