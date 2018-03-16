#include <iostream>

#include "tests/test.h"
//#include "OsnovaLab/Data/Packet.h"
#include "OsnovaLab/Tools/Net/PacketQueue.h"

namespace OsnovaLab
{
	namespace Tests
	{
		int test2()
        {

            Tools::Net::PacketQueue pack;

            char buff[1024] = { 0 };

            pack.Push("Hello", 5);


            pack.Push(", ", 2, true);
            pack.Push("World",5);

            auto a = pack.Count();

            auto pop = pack.Pop(buff, sizeof(buff), true);

            std::cout << "Pack count = " << a << ", pop result = "<< pop << std::endl;
            std::cout << buff << std::endl;


/*
            Data::Queue<Data::Packet> queue;

            queue.Push(Data::Packet("Hello", 5));

            auto p = queue.Front();

            queue.Push(Data::Packet(" World", 6));

            char buff[100];

            Data::Packet pack;

            queue.Pop(std::move(pack));
*/



/*
            Data::Packet pack;

            pack.Push("Hello", 5);
            pack.Push(", ", 2);
            pack.Push("World", 6);

            char buff[1024] = { 0 };
            auto af_size = pack.Size();
            auto a = pack.Pop(buff, 5);
            auto bf_size = pack.Size();


            std::cout << "Pop result = " << a << std::endl;
            std::cout << buff << std::endl;

*/

            return 0;
		}

        int test21()
        {
            //std::unique_ptr<char, void(*)(char*)> char_ptr((char*)std::malloc(100), [](char* p) { std::free(p);});
            //std::vector a;



        }
	}
}
