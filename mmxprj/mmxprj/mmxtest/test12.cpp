#include <iostream>
#include "mmxlib/tools/string/string_token.h"
#include "mmxlib/tools/timer.h"
#include "mmxlib/net/select.h"

#include <string>

namespace mmxtest
{

    int test12()
    {     

		mmx::tools::string::StringToken token(" ,. first .,.: two .,.. three .:,.,");

		const char* t = nullptr;

        while ((t = token.GetToken(" ,:")) != nullptr)
		{
			std::cout << t << std::endl;
		}

		std::cout.flush();

		std::cin.get();

    }

    int test12_1()
    {

        std::string s = std::string(3,'\xff');

        std::cout << s << std::endl;

        mmx::net::Select select;

        //select.Set(4, mmx::net::S_EV_READ);

        mmx::tools::Timer tim;

        tim.HStart(10);

        while (1)
        {
            auto el = tim.Elapsed();
            auto lft = tim.Left();

            std::cout << el << ", " << lft << std::endl;

            if (tim.IsEnable())
            {
                tim.HStart(10);
                std::cout << "Timer restart" << std::endl;
            }
            else
            {
                mmx::tools::Timer::Sleep(1);
            }

        }

        std::cout.flush();

        std::cin.get();

    }


}
