#include "mixer.h"

namespace mmx
{
    namespace media
    {
        namespace codecs
        {
            namespace audio
            {
                short mixer(short pcm1, short pcm2, unsigned char gain)
                {
                    int rc = ((int)pcm1 * (int)gain) + (int)pcm2 * (int)gain;
                    rc /= 100;

                    if (rc > 32767)
                    {
                        rc = 32767;
                    }
                    else if (rc < -32768)
                    {
                        rc = 32768;
                    }
                    return rc;
                }
            }
        }
    }
}
