#ifndef _MMX_TOOLS_MEDIA_MARSHALER_H
#define _MMX_TOOLS_MEDIA_MARSHALER_H

#include "imarshaler.h"
#include "data/idpwriter.h"

namespace mmx
{
    namespace tools
    {

        class MediaMarshaler : public IMarshaler
        {

            data::IDataPacketWriter&    dp_writer_;
            int                         packet_id_;

            // IMarshaler interface
        public:
            MediaMarshaler(data::IDataPacketWriter& dp_writer);
            MediaMarshaler(MediaMarshaler&& mmarshaler);
            ~MediaMarshaler();

            int PutData(const sniffers::IIPPacket& ippacket) override;
        };
    }

}

#endif
