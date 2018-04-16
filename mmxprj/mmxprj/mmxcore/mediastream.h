#ifndef _MMXMUX_MEDIA_STREAM_H
#define _MMXMUX_MEDIA_STREAM_H

#include <typeinfo>
#include <vector>

#include "mmxlib/headers/media.h"

namespace mmxmux
{
    class MediaPool;
    class MediaStream
    {
        std::vector<char*> data_;
        unsigned short pack_id_;
        int ref_count_;

        unsigned int address_;
        unsigned short port_;

        friend class MediaPool;

    public:

        MediaStream(unsigned int address = 0, unsigned short port = 0);
        MediaStream(MediaStream&& mediastream);
        MediaStream& operator=(MediaStream&& mediastream);

        int PutSample(const mmx::headers::MEDIA_DATA& media);
        const mmx::headers::MEDIA_SAMPLE* GetSample() const;
        void Clear();

    };
}

#endif
