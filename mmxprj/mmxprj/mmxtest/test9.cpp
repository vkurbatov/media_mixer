#include "mmxlib/media/codecs/audio/pcma.h"
#include "mmxlib/media/codecs/audio/mixer.h"

#include <cstring>
#include <iostream>


#define MEDIA_SIZE  10
#define SAMPLE_COUNT  8

namespace mmxtest
{

    class A{
        int val_;
    public:
        int get() { return val_; }
    };

    class B{
        int val_;
    public:
        B(int val = 0) : val_(val) {}
        //B& operator = (B&& b) { val_ = b.val_; return *this; }
        int get() { return val_; }
    };

    int test9()
    {     

        A a1, a2;

        a1 = a2;

        B b1;
        B b2(b1);

        b1 = b2;

        std::cout << "PCMA codec table:" << std::endl;
        mmx::media::codecs::audio::PcmaCodec pcma;
        int errors = 0;
        for (int i = 0; i < 256; i++)
        {
            short pcm = pcma.Decode((unsigned char)i);
            unsigned char alaw = pcma.Encode(pcm);
            short mix = mmx::media::codecs::audio::mixer(pcm, pcm, 75);

            errors += (unsigned char)i != alaw;

            std::cout << "PCMA[" << i << "] -> PCM[" << pcm << "] -> PCMA[" << (int)alaw << "] -> MIX_PCM[" << mix << "]" << std::endl;
        }
        std::cout << "Errors: " << errors << std::endl;
    }
}
