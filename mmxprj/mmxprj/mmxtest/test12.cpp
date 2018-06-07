#include <iostream>
#include "mmxlib/tools/string/string_token.h"

namespace mmxtest
{

    int test12()
    {     

		mmx::tools::string::StringToken token(" ,. first .,.: two .,.. three .:,.,");

		const char* t = nullptr;

		while ((t = token.GetToken(" ,.:")) != nullptr)
		{
			std::cout << t << std::endl;
		}

		std::cout.flush();

		std::cin.get();

    }
}
