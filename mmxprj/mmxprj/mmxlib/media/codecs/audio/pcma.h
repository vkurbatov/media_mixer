#ifndef _MMX_MEDIA_CODECS_AUDIO_PCMA_H
#define _MMX_MEDIA_CODECS_AUDIO_PCMA_H

#include "iaudiocodec.h"
#include <array>

namespace mmx
{
    namespace media
    {
        namespace codecs
        {
            namespace audio
            {
                class PcmaCodec : public IAudioCodec
                {

                    static std::array<unsigned char,65536>  alaw_table_;
                    static std::array<short,256>            pcm_table_;

                public:
                    PcmaCodec();
                    ~PcmaCodec();

                    static unsigned char Encode(short pcm_val);
                    static short Decode(unsigned char a_val);

                    int Encode(const void* idata,
                                   int isize,
                                   void *odata,
                                   int osize,
                                   void* context = nullptr) override;
                    int Decode(const void* idata,
                                   int isize,
                                   void *odata,
                                   int osize,
                                   void* context = nullptr) override;
                private:

                    static unsigned char lin2alaw(short l_val);
                    static short alaw2lin(unsigned char a_val);
                    static void fill_tables();

                };
            }
        }
    }
}

#endif
