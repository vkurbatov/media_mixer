#ifndef _MMX_TOOLS_SANGOMA_CLIENT_H
#define _MMX_TOOLS_SANGOMA_CLIENT_H

#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "tools/iosockadp.h"
#include "tools/deffwriter.h"
#include "headers/si.h"

#include <list>
#include <vector>

namespace mmx
{
    namespace tools
    {
        class SangomaServer;

        class SangomaClient : public IChannelDispatcher
        {

            net::Socket                     socket_;
            SocketIOAdapter                 adapter_;
            DeferredWriter                  writer_;

            net::SelectExtension&           select_;

            std::vector<char>               data_;
            const headers::SANGOMA_PACKET*  query_;

            friend class SangomaServer;
            SangomaClient(net::Socket&& socket, net::SelectExtension& select);

        public:

            SangomaClient(SangomaClient&& channel);

            ~SangomaClient() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int QueryOrderTimeout() const override;
            int Close() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            const headers::SANGOMA_PACKET* GetQuery();
            int PutAnswer(const headers::SANGOMA_PACKET& answer);
            void Drop();

        private:

            int putData(const void* data = nullptr, int size = 0);

        };
    }
}

#endif
