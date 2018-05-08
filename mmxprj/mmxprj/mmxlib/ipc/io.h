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
            virtual int Write(const void*, int, int = 0) = 0;
            virtual int Read(void*, int, int = 0) = 0;
            virtual bool IsCanWrite() const = 0;
            virtual bool IsCanRead() const = 0;
        };
	}
}

#endif
