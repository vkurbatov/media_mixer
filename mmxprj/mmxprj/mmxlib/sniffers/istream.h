#ifndef _MMX_SNIFFERS_I_STREAM_H
#define _MMX_SNIFFERS_I_STREAM_H

namespace mmx
{
    namespace sniffers
    {
        class IStream
        {
        public:
            virtual ~IStream(){}
            virtual int PutStream(void* stream, int size, void* hcontext = nullptr) = 0;
            virtual int Next() = 0;
            virtual int Reset() = 0;
            virtual bool IsComplete() const= 0;
            virtual bool IsBad() const= 0;
        };
    }
}

#endif
