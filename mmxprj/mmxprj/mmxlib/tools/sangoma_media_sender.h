#ifndef _MMX_TOOLS_SANGOMA_MEDIA_CLIENT_H
#define _MMX_TOOLS_SANGOMA_MEDIA_CLIENT_H


#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "tools/iosockadp.h"
#include "tools/deffwriter.h"
#include "tools/timer.h"
#include "headers/order645_2.h"
#include "headers/si.h"

#include <list>
#include <vector>

namespace mmx
{
    namespace tools
    {

        class SangomaMediaSender : public IChannelDispatcher
        {

            net::Socket                                 socket_;
            SocketIOAdapter                             adapter_;
            DeferredWriter                              writer_;
            tools::Timer                                timer_;

            mmx::headers::SANGOMA_MEDIA_STREAM_PACKET   sangoma_;



            net::SelectExtension&                       select_;

            std::vector<char>                           data_;

            int                                         interval_;

            net::address_t                              address_;
            net::port_t                                 port_;


        public:
            SangomaMediaSender(net::address_t address, net::port_t port, net::SelectExtension& select, int interval = 2000);
            SangomaMediaSender(SangomaMediaSender&& channel);

            ~SangomaMediaSender() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int QueryOrderTimeout() const override;
            int Close() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            int PutMedia(const void* data, int size, unsigned char mcl_a, unsigned char mcl_b);
            DeferredWriter& GetWritter();

        private:

            int putData(const void* data = nullptr, int size = 0);

            int combineSend(const void* data, int size, unsigned char mcl);
            int separatedSend(const void* data, int size, unsigned char mcl_a, unsigned char mcl_b);

            int checkConnect();
            int checkData();


        };
    }
}

#endif
