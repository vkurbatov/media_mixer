#ifndef _MMX_MEDIA_CODECS_AUDIO_I_AUDIO_CODEC_H
#define _MMX_MEDIA_CODECS_AUDIO_I_AUDIO_CODEC_H

namespace mmx
{
    namespace media
    {
        namespace codecs
        {
            namespace audio
            {
                class IAudioCodec
                {

                public:
                    virtual ~IAudioCodec() {};
                    virtual int Encode(const void* idata,
                                       int isize,
                                       void *odata,
                                       int osize,
                                       void* context = nullptr) = 0;
                    virtual int Decode(const void* idata,
                                       int isize,
                                       void *odata,
                                       int osize,
                                       void* context = nullptr) = 0;
                };
            }
        }
    }
}

#endif
