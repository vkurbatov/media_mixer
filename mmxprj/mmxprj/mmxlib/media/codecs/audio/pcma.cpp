
#include "pcma.h"

#include <errno.h>

namespace mmx
{
    namespace media
    {
        namespace codecs
        {
            namespace audio
            {
                std::array<unsigned char,65536> PcmaCodec::alaw_table_;
                std::array<short,256> PcmaCodec::pcm_table_;

                PcmaCodec::PcmaCodec()
                {
                    std::array<unsigned char,65536> arr;
                }

                PcmaCodec::~PcmaCodec()
                {

                }

                unsigned char PcmaCodec::lin2alaw(short l_val)
                {

                    unsigned char	rc;

                    const static short segs[8] = {0xFF, 0x1FF, 0x3FF, 0x7FF,
                                0xFFF, 0x1FFF, 0x3FFF, 0x7FFF};

                    unsigned short  mask = 0xD5;

                    int             seg = 0;

                    if (l_val < 0)
                    {
                        mask = 0x55;
                        l_val = -l_val - 8;
                        if (l_val < 0)
                        {
                            l_val = 0;
                        }
                    }


                    for (;seg < 8 && l_val > segs[seg]; seg++);


                    if (seg >= 8)
                    {
                        rc = (0x7F ^ mask);
                    }
                    else
                    {
                        rc = (seg << 4);

                        if (seg < 2)
                        {
                            rc |= (l_val >> 4) & 0x0F;
                        }
                        else
                        {
                            rc |= (l_val >> (seg + 3)) & 0x0F;
                        }

                        rc ^= mask;
                    }

                    return rc;
                }

                short PcmaCodec::alaw2lin(unsigned char a_val)
                {

                    a_val ^= 0x55;

                    int seg = ((unsigned char)a_val & 0x70) >> 4;

                    short rc = ((a_val & 0x0F) << 4) + 0x08;

                    rc += 0x100 * (short)(seg > 0);
                    rc <<= (seg - 1)*(short)(seg > 1);

                    return ((a_val & 0x80) ? rc : -rc);
                }

                void PcmaCodec::fill_tables()
                {
                    static bool fill = false;
                    if (!fill)
                    {
                        for (int i = 0; i < 65536; i++)
                        {
                            alaw_table_[i] = lin2alaw((short)i);
                            //alaw_table_[i] = pjmedia_linear2alaw((short)i);
                        }

                        for (int i = 0; i < 256; i++)
                        {
                            pcm_table_[i] = alaw2lin(i);
                            //pcm_table_[i] = pjmedia_alaw2linear(i);

                        }
                        fill = true;
                    }
                }

                unsigned char PcmaCodec::Encode(short pcm_val)
                {
                    fill_tables();
                    return alaw_table_[(unsigned short)pcm_val];
                }

                short PcmaCodec::Decode(unsigned char a_val)
                {
                    fill_tables();
                    return pcm_table_[a_val];
                }

                int PcmaCodec::Encode(const void* idata,
                               int isize,
                               void *odata,
                               int osize,
                               void* context)
                {
                    int rc = -EINVAL;

                    isize /= 2;

                    if (idata != nullptr && odata != nullptr && isize > 0 && osize > 0)
                    {
                        short* pcm = (short*)idata;
                        char* a_law = (char*)odata;
                        for (rc = 0; rc < isize && rc < osize; rc++)
                        {
                            a_law[rc] = lin2alaw(pcm[rc]);
                        }
                    }

                    return rc;
                }

                int PcmaCodec::Decode(const void* idata,
                               int isize,
                               void *odata,
                               int osize,
                               void* context)
                {
                    int rc = -EINVAL;

                    osize /= 2;

                    if (idata != nullptr && odata != nullptr && isize > 0 && osize > 0)
                    {
                        short* pcm = (short*)odata;
                        char* a_law = (char*)idata;
                        for (rc = 0;rc < isize && rc < osize; rc++)
                        {
                            pcm[rc] = alaw2lin(a_law[rc]);
                        }
                    }

                    return rc;
                }
            }
        }
    }
}

