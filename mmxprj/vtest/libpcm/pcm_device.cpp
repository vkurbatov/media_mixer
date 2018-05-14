#include <cstring>
#include "pcm_device.h"

namespace pcm
{

    PcmDevice::PcmDevice(const char* device_name,
                         snd_pcm_stream_t stream_type,
                         snd_pcm_format_t pcm_format,
                         snd_pcm_access_t pcm_access,
                         int channels,
                         unsigned int rate,
                         int mode) :
        handle_(nullptr),
        width_(0)
    {
        pcm_config_.stream_type = stream_type;
        pcm_config_.pcm_format = pcm_format;
        pcm_config_.access = pcm_access;
        pcm_config_.channels = channels;
        pcm_config_.rate = rate;
        pcm_config_.mode = mode;


        if (device_name == nullptr || *device_name == '\0')
        {
            device_name = DEFAULT_PCM_DEVICE_NAME;
        }

        std::strcpy(pcm_config_.device_name, device_name);
    }

    PcmDevice::~PcmDevice()
    {
        Close();
    }

    int PcmDevice::Open()
    {
        int rc = -EBUSY;

        if (handle_ == nullptr)
        {
            rc = snd_pcm_open(&handle_, pcm_config_.device_name, pcm_config_.stream_type, pcm_config_.mode);

            if (rc >= 0)
            {
                rc = applySetting();
                if (rc < 0)
                {
                    Close();
                }
            }
        }

        return rc;
    }

    int PcmDevice::Close()
    {
        int rc = -EBADF;

        if (handle_ != nullptr)
        {
            rc = snd_pcm_close(handle_);
            handle_ = nullptr;
        }

        return rc;

    }

    int PcmDevice::Handle() const
    {
        return *(int*)handle_;
    }

    int PcmDevice::Read(void *data, int size)
    {
        int rc = -EINVAL;

        if (handle_ != nullptr && data != nullptr && size > 0)
        {
            rc = snd_pcm_readi(handle_, data, size / pcm_config_.channels / width_);
        }

        return rc;
    }

    int PcmDevice::Write(const void *data, int size)
    {

    }

    int PcmDevice::SetConfig(const pcm_config_t& cfg)
    {
        int rc = -1;

        if (handle_ == nullptr)
        {
            pcm_config_ = cfg;
            rc = 0;
        }

        return rc;
    }

    const PcmDevice::pcm_config_t& PcmDevice::GetConfig() const
    {
        return pcm_config_;
    }

    int PcmDevice::applySetting()
    {
        int rc = -1;

        if (handle_ != nullptr)
        {
            rc = snd_pcm_set_params(handle_,
                                    pcm_config_.pcm_format,
                                    pcm_config_.access,
                                    pcm_config_.channels,
                                    pcm_config_.rate,
                                    DEFAULT_PCM_RESAMPLE,
                                    DEFAULT_PCM_LATENCY);
            if (rc >= 0)
            {
                width_ = snd_pcm_format_width(pcm_config_.pcm_format) / 8;
            }
        }
/*
        enum : int
        {
            PCM_CFG_INIT,
            PCM_CFG_ACCESS,
            PCM_CFG_FORMAT,
            PCM_CFG_RATE,
            PCM_CFG_CHANNELS,
            PCM_CFG_SET,
            PCM_CFG_END
        }cfg_state = PCM_CFG_INIT;

        if (handle_ != nullptr)
        {
            snd_pcm_hw_params_t *hw_params = nullptr;

            rc = snd_pcm_hw_params_malloc(&hw_params);

            if (rc >= 0)
            {

                cfg_state = rc < 0 ? PCM_CFG_END : PCM_CFG_INIT;

                while (cfg_state != PCM_CFG_END)
                {
                    switch(cfg_state)
                    {
                        case PCM_CFG_INIT:

                            rc = snd_pcm_hw_params_any(handle_, hw_params);

                            break;
                        case PCM_CFG_ACCESS:

                            rc = snd_pcm_hw_params_set_access(handle_, hw_params, pcm_config_.access);

                            break;
                        case PCM_CFG_FORMAT:

                            rc = snd_pcm_hw_params_set_format(handle_, hw_params, pcm_config_.pcm_format);

                            break;
                        case PCM_CFG_RATE:
                            {
                                //unsigned int pcm_config_ = cfg.rate;
                                rc = snd_pcm_hw_params_set_rate_near(handle_, hw_params, &pcm_config_.rate, nullptr);
                            }
                            break;
                        case PCM_CFG_CHANNELS:

                            rc = snd_pcm_hw_params_set_channels(handle_, hw_params, pcm_config_.channels);

                            break;
                        case PCM_CFG_SET:

                            rc = snd_pcm_hw_params (handle_, hw_params);

                            break;
                        case PCM_CFG_END:

                            break;
                    }

                    (*(int*)&cfg_state)++;

                }

                snd_pcm_hw_params_free(hw_params);

                rc = snd_pcm_prepare(handle_);

            }
        }
        */
        return rc;
    }

}
