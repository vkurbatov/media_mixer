#ifndef _OL_FILE_IO_H
#define _OL_FILE_IO_H

#include <string>
#include <fstream>

#include "OsnovaLab/IO/AbstractDeviceIO.h"

namespace OsnovaLab
{
	namespace IO
	{
		class FileIO : public AbstractDeviceIO
		{
			std::string	filename_;
			std::fstream filestream_;
			std::ios_base::openmode mode_;
		
			
		public:
			FileIO(const std::string& filename, std::ios_base::openmode mode, Thread::IMutex* mutex = nullptr);
			virtual ~FileIO();

		protected:			
            virtual const data_size_t ReadIO(p_data_t data, data_size_t size);
            virtual const data_size_t WriteIO(const p_data_t data, data_size_t size);


		private:
			inline void MutexLock();
			inline void MutexUnlock();
		};
	}
}


#endif
