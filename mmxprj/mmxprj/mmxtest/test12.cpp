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


        const int delay = 50;

        int cnt = 0;
        int dti = 0;

        int max_dt = 0;

        tim.Start(delay);

        unsigned int start = mmx::tools::Timer::GetTicks();
        unsigned int tick = mmx::tools::Timer::GetTicks();

        while (1)
        {
            //auto el = tim.Elapsed();
            //auto lft = tim.Left();

            //std::cout << el << ", " << lft << std::endl;

            int rc = select.Wait(tim.Left());

            if (tim.IsEnable())
            {

                int dt = mmx::tools::Timer::GetTicks() - tick;

                cnt ++;
                dti = mmx::tools::Timer::GetTicks() - start;

                if (dt > max_dt)
                    max_dt = dt;

                //if (dt != delay)
                {
                    std::cout << "Timer restart: " << mmx::tools::Timer::GetTicks() << ", dt = " << dt << ", " << max_dt << ", error = " << ((double)dti / (double)delay) - (double)cnt << std::endl;
                }

                tim.Start(delay);
                tick = mmx::tools::Timer::GetTicks();
                //std::cout << "Timer restart: " << mmx::tools::Timer::GetTicks() << std::endl;
            }
            else
            {
                //mmx::tools::Timer::Sleep(1);
            }

        }

        std::cout.flush();

        std::cin.get();

    }


}
