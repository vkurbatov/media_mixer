#include "jitterbuffer.h"

namespace mmx
{
    namespace media
    {
        JitterBuffer::JitterBuffer(int jitter) :
            jitter_(jitter),
            samples_(0),
            current_(-1)
        {

        }

        JitterBuffer::JitterBuffer(JitterBuffer&& jitter_buffer) :
            jitter_(jitter_buffer.jitter_),
            samples_(std::move(jitter_buffer.samples_))
        {

        }

        JitterBuffer::~JitterBuffer()
        {

        }

        int JitterBuffer::PutMedia(const headers::MEDIA_DATA& media)
        {
            int rc = 0;

            return rc;
        }

        const Sample* JitterBuffer::GetSample() const
        {

        }

        void JitterBuffer::Clear()
        {
            current_ = -1;
        }
    }
}
