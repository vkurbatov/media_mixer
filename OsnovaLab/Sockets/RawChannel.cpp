
#include "OsnovaLab/Sockets/RawChannel.h"
#include "OsnovaLab/Sockets/RawConnection.h"

#ifndef SIO_RCVALL

#define SIO_RCVALL 0x98000001

#endif

namespace OsnovaLab
{
	namespace Sockets
	{
			RawChannel::RawChannel() : AbstractChannel()
			{

			}

			IConnection* RawChannel::Open(const IConnectionString& connectionString)
			{
				IConnection* rc = nullptr;

				SocketPtr socket = MakeSocket(CreateSocket(SOCKET_RAW, IPPROTO_UDP));

                auto e = errno;
                if (socket->IsValid())
                {
                    sockaddr sa = { 0 };
                    sa.sa_family = AF_INET;

                    auto s_rc = ::bind(*socket, (sockaddr*)&sa, sizeof(sockaddr));
                    e = errno;

                    u_long flag = 1;
                    int one = 1;

                    const int *val = &one;
                    //s_rc = ::setsockopt(*socket, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));

                    addChannel(rc);
                    rc = new RawConnection(socket);
                }

                /*
				char        name[128];
				gethostname(name, sizeof(name));
                HOSTENT*    phe;
				phe = gethostbyname(name);
				/*
				sockaddr_in sa = {};
				sa.sin_family = AF_INET;
				sa.sin_addr.S_un.S_addr = ((struct in_addr *)phe->h_addr_list[3])->s_addr;
				::bind(*socket, (sockaddr*)&sa, sizeof(sockaddr_in));

				*/




				return rc;
			}

			socket_result_t RawChannel::Close(IConnection* server)
			{
				return removeChannel(server);
			}

	}
}
