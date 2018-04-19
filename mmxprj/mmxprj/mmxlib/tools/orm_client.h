#ifndef _MMX_TOOLS_ORM_CLIENT_H
#define _MMX_TOOLS_ORM_CLIENT_H

#include "ichanneldispatcher.h"
#include "net/socket.h"
#include "net/select_ex.h"
#include "tools/iosockadp.h"
#include "tools/deffwriter.h"
#include "headers/order645_2.h"

#include <list>
#include <vector>

namespace mmx
{
    namespace tools
    {
        class OrmServer;

        class OrmClient : public IChannelDispatcher
        {

            net::Socket                     socket_;
            SocketIOAdapter                 adapter_;
            DeferredWriter                  writer_;

            net::SelectExtension&           select_;

            std::vector<char>               data_;

            int                             recieve_data_;

            friend class OrmServer;
            OrmClient(net::Socket&& socket, net::SelectExtension& select);

        public:

            OrmClient(OrmClient&& channel);

            ~OrmClient() override;
            int Dispatch(dispatch_flags_t dispatch, void* context = nullptr) override;
            int QueryOrderTimeout() const override;
            int Close() override;
            bool IsDown() const override;
            bool IsReadyData() const override;

            const std::vector<char>& GetQuery() const;
            void Drop();

            int PutData(const mmx::headers::ORM_INFO_PACKET& orm_info);

        private:

            int putData(const void* data = nullptr, int size = 0);

        };
    }
}

#endif
