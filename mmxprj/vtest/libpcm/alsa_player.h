#ifndef _MMX_MEDIA_ALSA_PLAYER_H
#define _MMX_MEDIA_ALSA_PLAYER_H

namespace mmx
{
	namespace media
	{
		class AlsaPlayer
		{
		public:
			AlsaPlayer();
			~AlsaPlayer();
			int Play(const void* media, int size);
		};
	}
}

#endif
