
#include <iostream>

#include "OsnovaLab/IO/ScreenIO.h"
#include <thread>
namespace OsnovaLab
{
	namespace IO
	{
		ScreenIO::ScreenIO(Thread::IMutex* mutex) : AbstractDeviceIO(mutex)
		{

		}

		ScreenIO::~ScreenIO()
		{

		}

        const data_size_t ScreenIO::ReadIO(p_data_t data, data_size_t size)
		{
			return 0;
		}

        const data_size_t ScreenIO::WriteIO(const p_data_t data, data_size_t size)
		{
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << (const char*)data;
            std::cout.flush();
			return size;
		}

		/*void ScreenIO::ReleaseMutex(Thread::IMutex* mutex)
		{
			// ничего не делаем. ћьютекс дл€ экрана один!!
		}
		*/
	}
}
