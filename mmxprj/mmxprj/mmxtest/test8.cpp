#include "mmxlib/media/jitterbuffer.h"
#include "mmxlib/sniffers/rtppacketwrapper.h"
#include <cstring>
#include <chrono>
#include <thread>

#include <iostream>


#define MEDIA_SIZE  10
#define SAMPLE_COUNT  8

namespace mmxtest
{
    int test8()
    {

        union
        {
            mmx::headers::RTP_PACKET rtp;
            char raw[sizeof(mmx::headers::RTP_HEADER) + MEDIA_SIZE];
        }rtp_packets[SAMPLE_COUNT];

        rtp_packets[0].rtp.header.csrc_count = 0;
        rtp_packets[0].rtp.header.extension = 0;
        rtp_packets[0].rtp.header.marker = 0;
        rtp_packets[0].rtp.header.packet_id = 1;
        rtp_packets[0].rtp.header.padding = 0;
        rtp_packets[0].rtp.header.pyload_type = 8;
        rtp_packets[0].rtp.header.ssrc = 0x01020304;
        rtp_packets[0].rtp.header.ver = mmx::headers::RTP_VERSION;
        rtp_packets[0].rtp.header.timestamp = std::rand();

        for (int i = 1; i < SAMPLE_COUNT; i++)
        {
            rtp_packets[i].rtp.header = rtp_packets[i - 1].rtp.header;
            rtp_packets[i].rtp.header.packet_id++;
            rtp_packets[i].rtp.header.timestamp += 160;
        }

        for (int i = 0; i < SAMPLE_COUNT; i++)
        {
            for (int j = 0; j < MEDIA_SIZE; j++)
            {
                rtp_packets[i].rtp.data[j] = i * MEDIA_SIZE + j;
            }
        }

        mmx::media::JitterBuffer jitter(20, 8000, SAMPLE_COUNT);

        int timestamp = mmx::media::Sample::GetCurrentTimestamp();

        for (int i = 0; i < SAMPLE_COUNT; i++)
        {

            //int j = i % 2 == 0 ? i + 1 : i - 1;

            int j = i;

            mmx::sniffers::RTPPacketWrapper rtp(rtp_packets[j].raw, sizeof(rtp_packets[j].raw));

            jitter.PutMedia(rtp, 1001 + j, timestamp + i * 20);
        }

        //jitter.Drop(timestamp + 20);

        for (int i = 0; i < SAMPLE_COUNT; i++)
        {
            const mmx::media::Sample* smpl = jitter.GetSample();

            std::cout << "Sample #" << i << " result:" << std::endl;

            if (smpl == nullptr)
            {
                std::cout << "  Sample #" << i << " not enought!!!." << std::endl;
            }
            else
            {

                const mmx::headers::MEDIA_SAMPLE* media = smpl->GetMediaSample();

                if (media == nullptr)
                {
                    std::cout << "  Sample #" << i << " is empty!!!." << std::endl;
                }
                else
                {
                    std::cout << std::endl;
                    std::cout << "Packet Id = " << media->header.packet_id << " ." << std::endl;
                    std::cout << "Timestamp = " << media->header.timestamp << " ." << std::endl;
                    std::cout << "Rtp packet Id = " << media->header.rtp_header.packet_id << " ." << std::endl;
                    std::cout << "Rtp timestamp = " << media->header.rtp_header.timestamp << " ." << std::endl << std::endl;

                    std::cout << "Media data (" << media->header.length - sizeof(media->header) << " bytes):" << std::endl << std::endl;

                    for (int i = 0; i < media->header.length - sizeof(media->header); i++)
                    {
                        if (i > 0)
                        {
                            std::cout << ", ";
                        }

                        std::cout << (int)media->media[i];
                    }

                    std::cout << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

        }

    }
}
