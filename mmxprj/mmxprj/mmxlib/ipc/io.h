#ifndef _MMX_IPC_IO_H
#define _MMX_IPC_IO_H

namespace mmx
{
	namespace ipc
	{
		class IIO
		{
		public:
            virtual ~IIO() {};
			virtual int Write(const char*, int, int = 0) = 0;
			virtual int Read(char*, int, int = 0) = 0;
        };
	}
}

#endif
