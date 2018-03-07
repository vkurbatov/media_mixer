#ifndef _OL_IO_H
#define _OL_IO_H

namespace OsnovaLab
{
	namespace IO
	{
        using data_t        = void;
        using p_data_t      = data_t*;
        using data_result_t = int;
        using data_size_t   = long;

		class IDeviceIO
		{
		public:
            virtual ~IDeviceIO() {};
            virtual const data_size_t Read(p_data_t data, data_size_t size) = 0;
            virtual const data_size_t Write(const p_data_t data, data_size_t size) = 0;
		};
	}
}


#endif
