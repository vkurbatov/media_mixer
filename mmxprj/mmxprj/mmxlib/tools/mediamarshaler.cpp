#include "mediamarshaler.h"

#include "headers/media.h"
#include "headers/udp.h"

#include <memory>       //std::move

#include <sys/time.h>   // gettimeofday
#include <errno.h>
#include <netdb.h>      // IPPROTO_UDP



namespace mmx
{
    namespace tools
    {

        MediaMarshaler::MediaMarshaler(data::IDataPacketWriter &dp_writer) :
            dp_writer_(dp_writer),
            packet_id_(0)
        {

        }

        MediaMarshaler::MediaMarshaler(MediaMarshaler &&mmarshaler) :
            dp_writer_(dp_writer_),
            packet_id_(mmarshaler.packet_id_)
        {

        }

        MediaMarshaler::~MediaMarshaler()
        {

        }

        int MediaMarshaler::PutData(const sniffers::IIPPacket &ippacket)
        {

            return 0;

        }
    }
}
