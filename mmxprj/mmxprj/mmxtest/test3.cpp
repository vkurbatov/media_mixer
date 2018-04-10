#include "defines.h"
#include <iostream>

#include "mmxlib/data/datapacket.h"
#include "mmxlib/tools/timer.h"

#define QUERY_SIZE 10

namespace mmxtest
{
    int test3()
    {
        mmx::data::DataPacket datapack(40);

        datapack.BuildPacket(1);

        auto a1 = datapack.QueryBlock(QUERY_SIZE);
        datapack.Commit();
        auto a2 = datapack.QueryBlock(QUERY_SIZE);
        datapack.Commit();
        auto a3 = datapack.QueryBlock(QUERY_SIZE);
        datapack.Commit();

        for (int i = 0; i < QUERY_SIZE; i++)
        {
            a1->data[i] = 10 + i;
            a2->data[i] = 20 + i;
        }

        int j = 1;

        while((a1 = datapack.GetBlock()) != nullptr)
        {
            std::cout << "Print #" << j++ << " block:" << std::endl;
            for (int i = 0; i < a1->header.length - sizeof(a1->header); i++)
            {
                std::cout << (int)a1->data[i] << ", ";
            }

            std::cout << std::endl;
        }

        datapack.Reset();

    }

    int test31()
    {
        mmx::tools::Timer tim;

        tim.Start(1000);

        auto e = tim.IsEnable();
        auto s = tim.IsStarted();
        auto el = tim.Elapsed();

        mmx::tools::Timer::Sleep(1000);

        e = tim.IsEnable();
        s = tim.IsStarted();
        el = tim.Elapsed();

        tim.Stop();

        e = tim.IsEnable();
        s = tim.IsStarted();
        el = tim.Elapsed();

        mmx::tools::Timer::Sleep(1000);

    }
}
