

#include "OsnovaLab/IO/FileIO.h"

namespace OsnovaLab
{
	namespace IO
	{
		FileIO::FileIO(const std::string& filename, std::ios_base::openmode mode, Thread::IMutex* mutex) :
			AbstractDeviceIO(mutex), 
			filename_(filename),
			mode_(mode)
		{			
			filestream_.open(filename_, mode_);
		}

		FileIO::~FileIO()
		{
			try
			{
				if (filestream_.is_open())
					filestream_.close();
			}
			catch (std::exception& e)
			{
				//TODO: думаем что будем делать с исключениями
				throw;
			}
		}

        const data_size_t FileIO::ReadIO(p_data_t data, data_size_t size)
		{
            data_size_t rc = -1;

			if (filestream_.is_open())
			{
				rc = -2;
                filestream_.read((char*)data, size);
                rc = filestream_.gcount();
			}

			return rc;
		}

        const data_size_t FileIO::WriteIO(const p_data_t data, data_size_t size)
		{
            data_size_t rc = -1;

			if (filestream_.is_open())
			{
				rc = -2;

                filestream_.write((const char*)data, size);
                filestream_.flush();
                rc = filestream_.gcount();

					// TODO:
			}

			return rc;
		}

	}
}
