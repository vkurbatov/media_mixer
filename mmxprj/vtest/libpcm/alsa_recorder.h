#ifndef _MMX_MEDIA_ALSA_RECORDER_H
#define _MMX_MEDIA_ALSA_RECORDER_H

namespace mmx
{
	namespace media
	{
		class AlsaRecorder
		{
		public:
			AlsaRecorder();
			~AlsaRecorder();
			int Record(void* media, int size);
		};
	}
}

#endif
