#ifndef _MMX_MEDIA_PCM_DEVICE_H
#define _MMX_MEDIA_PCM_DEVICE_H


#include <alsa/asoundlib.h>

#define PCM_DEVICE_NAME_LEN         256
#define DEFAULT_PCM_DEVICE_NAME     "default"
#define DEFAULE_PCM_STREAM_TYPE     SND_PCM_STREAM_PLAYBACK
#define DEFAULE_PCM_FORMAT          SND_PCM_FORMAT_A_LAW
#define DEFAULE_PCM_ACCESS          SND_PCM_ACCESS_RW_INTERLEAVED
#define DEFAULT_PCM_MODE            0
#define DEFAULT_PCM_CHANNELS        1
#define DEFAULT_PCM_RATE            8000
#define DEFAULT_PCM_RESAMPLE        1
#define DEFAULT_PCM_LATENCY         500000


namespace pcm
{
    class PcmDevice
    {
        snd_pcm_t           *handle_;
        struct pcm_config_t
        {
            snd_pcm_format_t    pcm_format;
            snd_pcm_stream_t    stream_type;
            snd_pcm_access_t    access;
            int                 mode;
            int                 channels;
            unsigned int                 rate;
            char                device_name[PCM_DEVICE_NAME_LEN];
        }pcm_config_;

        int                     width_;

    public:
        PcmDevice(const char* device_name = nullptr,
                  snd_pcm_stream_t stream_type = DEFAULE_PCM_STREAM_TYPE,
                  snd_pcm_format_t pcm_format = DEFAULE_PCM_FORMAT,
                  snd_pcm_access_t pcm_access = DEFAULE_PCM_ACCESS,
                  int channels = DEFAULT_PCM_CHANNELS,
                  unsigned int rate = DEFAULT_PCM_RATE,
                  int mode = DEFAULT_PCM_MODE
                  );
        ~PcmDevice();
        int Open();
        int Close();
        int Handle() const;
        int Read(void* data, int size);
        int Write(const void* data, int size);
        int SetConfig(const pcm_config_t& cfg);
        const pcm_config_t& GetConfig() const;
    private:
        int applySetting();

    };
}

#endif
