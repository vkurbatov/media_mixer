#include "server.h"

#include <cstring>

#include <iostream>

#include <netdb.h>                  // SOCK_RAW, IPPROTO_UDP
#include <fcntl.h>                  // O_NONBLOCK
#include <errno.h>
#include <sys/time.h>               // gettimeofday

#include "mmxlib/logs/log.h"
#include "mmxlib/names.h"

#include "mmxlib/data/dpreader.h"


#define DEFAULT_TIMEOUT 2000

namespace mmxsrv
{
    Server::Server(const SERVER_CONFIG& config) :
        input_channel_(MMX_SERVER_CHANNEL_PATTERN, config.channel, select_, config.interval),
        orm_server_(config.pult ? config.address : 0, config.pult ? config.port : 0, select_, config.interval),
        sangoma_(config.pult ? 0 : config.address, config.pult ? 0 : config.address, select_, config.interval),
        config_(config)
    {

    }

    int Server::Execute()
    {

        int rc = 0;

        init();

        while(1)
        {

            rc = select_.Wait(getTimeouts());

            if (rc >= 0 || rc == -ETIMEDOUT)
            {
                if (rc == -ETIMEDOUT)
                {
                    dispatchAll(mmx::tools::DISPATCH_TIMER);
                }
                else
                {

                    dispatchAll(mmx::tools::DISPATCH_IO);
                    processInput();
                }

            }
            else
            {
                closeAll();
            }
        }


        return rc;

    }

    void Server::init()
    {
        dispatchAll(mmx::tools::DISPATCH_INIT);
    }

    void Server::dispatchAll(mmx::tools::dispatch_flags_t dispatch)
    {
        input_channel_.Dispatch(dispatch);

        if (config_.pult)
        {

            orm_server_.Dispatch(dispatch);

            for (auto& c : orm_server_.GetClients())
            {
                c.Dispatch(dispatch);
            }

            orm_server_.GetClients().remove_if([](mmx::tools::OrmClient& c) { return c.IsDown(); });

        }
        else
        {
            sangoma_.Dispatch(dispatch);
        }
    }

    int Server::getTimeouts()
    {
        int rc = mmx::net::INFINITE_TIMEOUT;

        int to = input_channel_.QueryOrderTimeout();

        if (to >= 0 && to < rc)
        {
            rc = to;
        }

        to = orm_server_.QueryOrderTimeout();

        if (to >= 0 && to < rc)
        {
            rc = to;
        }

        to = sangoma_.QueryOrderTimeout();

        if (to >= 0 && to < rc)
        {
            rc = to;
        }

        return rc;

    }

    void Server::processInput()
    {
        auto data = (const char*)input_channel_.Data();
        int size = input_channel_.Size();

        if (data != nullptr && size > 0)
        {

            int ret = 1;

            int off = 0;

            while (ret > 0 && off < size)
            {

                ret = dp_sniffer_.PutStream(data + off, size - off);

                if (dp_sniffer_.IsComplete())
                {

                    auto dp = dp_sniffer_.GetDataPacket();

                    if (dp != nullptr)
                    {

                        mmx::data::DataPacketReader reader(&dp->header);


                        auto block = reader.GetBlock();

                        while(block != nullptr)
                        {


                            if (block->header.length >= sizeof(mmx::headers::ORM_INFO_HEADER))
                            {

                                mmx::headers::ORM_INFO_PACKET& orm = *(mmx::headers::ORM_INFO_PACKET*)block->data;

                                if (config_.pult)
                                {

                                    int total_size = block->header.length - sizeof(block->header);

                                    std::memcpy(&orm_info_, &orm, total_size);
                                    std::memset((char*)&orm_info_ + total_size,
                                                0x7F,
                                                mmx::headers::ORDER_645_2_MAX_DATA_SIZE
                                                - (total_size - sizeof(orm_info_.header)));

                                    for (auto& c : orm_server_.GetClients())
                                    {
                                        c.PutData(orm_info_);
                                    }
                                }
                                else
                                {
                                    sangoma_.PutData(orm);
                                }
                            }


                            block = reader.GetBlock();
                        }
                    }

                    dp_sniffer_.Drop();

                }

                off += ret > 0 ? ret : 0;

            }

            input_channel_.Drop();

        }
    }


    void Server::closeAll()
    {

        input_channel_.Close();

        sangoma_.Close();

        orm_server_.GetClients().clear();

        orm_server_.Close();

    }


}
