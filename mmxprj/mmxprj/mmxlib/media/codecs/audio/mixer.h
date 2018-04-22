#ifndef _MMX_MEDIA_CODECS_AUDIO_MIXER_H
#define _MMX_MEDIA_CODECS_AUDIO_MIXER_H


namespace mmx
{
    namespace media
    {
        namespace codecs
        {
            namespace audio
            {
                short mixer(short pcm1, short pcm2, unsigned char gain = 75);
            }
        }
    }
}

#endif
