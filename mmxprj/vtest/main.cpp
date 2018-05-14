#include <iostream>
#include <cstring>

#include "libpcm/pcm_device.h"
#include <fcntl.h>  //O_NONBLOCK

#define SAMPLE_WIDTH    160

int main(int argc, char* argv[])
{


    unsigned char buff[SAMPLE_WIDTH];
    std::memset(buff, 0, sizeof(buff));



    pcm::PcmDevice record("default",
                          SND_PCM_STREAM_CAPTURE,
                          SND_PCM_FORMAT_A_LAW,
                          SND_PCM_ACCESS_RW_INTERLEAVED,
                          1,
                          8000,
                          O_NONBLOCK);

    int rc = record.Open();

    while (rc >= 0)
    {
        rc = record.Read(buff, SAMPLE_WIDTH);

        if (rc >= 0)
        {
            //std::cout << "Reading " << " bytes from record device:" << std::endl;
            for (int i = 0; i < rc; i++)
            {
                if (buff[i] != 213)
                {
                    std::cout << (int)buff[i] << ", ";
                }
            }

            std::cout << std::endl;
        }
    }

    record.Close();

    return 0;
}
