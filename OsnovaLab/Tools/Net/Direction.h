#ifndef _OL_TOOLS_NET_DIRECTION_H
#define _OL_TOOLS_NET_DIRECTION_H

#include "OsnovaLab/Tools/Net/IDirection.h"
#include "OsnovaLab/Tools/Net/PacketQueue.h"
#include "OsnovaLab/Thread/StdMutex.h"
#include "OsnovaLab/Functional/Caller.h"

namespace OsnovaLab
{
    namespace Tools
    {
        namespace Net
        {



            class Direction: public IDirection
            {

                Thread::StdMutex                 rd_mutex_;
                Thread::StdMutex                 wr_mutex_;
                Sockets::IClient&                client_;


                PacketQueue                     send_queue_;
                PacketQueue                     recv_queue_;


            public:

                Direction(Sockets::IClient& client);
                ~Direction() override;

                //IDirection
                virtual Sockets::socket_result_t Dispatch(const Sockets::event_mask_t& eventMask) override;
                virtual Sockets::socket_size_t GetSendQueueStatus() const;
                virtual Sockets::socket_size_t GetRecvQueueStatus() const;
                //virtual Sockets::IClient* GetClient() override;

                //IClient
                virtual Sockets::client_size_t Send(const Sockets::client_data_ptr_t data, Sockets::client_size_t size) override;
                virtual Sockets::client_size_t Recv(Sockets::client_data_ptr_t data, Sockets::client_size_t size) override;
                virtual const Sockets::SocketPtr& GetHandle() const override;               
            };
        }
    }
}

#endif
