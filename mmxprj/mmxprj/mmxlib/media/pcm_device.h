#ifndef _MMX_MEDIA_PCM_DEVICE_H
#define _MMX_MEDIA_PCM_DEVICE_H

namespace mmx
{
	namespace media
	{
		class PcmDevice
		{
		public:
			PcmDevice();
			~PcmDevice();
			int Open();
			int Close();
		};
	}
}

#endif
