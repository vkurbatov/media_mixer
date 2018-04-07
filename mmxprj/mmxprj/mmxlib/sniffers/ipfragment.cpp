#include "ipfragment.h"

#include "headers/ip.h"

#include <errno.h>

#include <algorithm>

namespace mmx
{
    namespace sniffers
    {
        IPFragment::IPFragment() :
            data_(DEFAULT_MTU_SIZE),
            total_bytes_(0),
            saved_bytes_(0),
            fragment_(false),
            packet_(data_.data()),
            last_packet_(false)
        {

        }

        IPFragment::IPFragment(IPFragment&& fragment):
            data_(std::move(fragment.data_)),
            total_bytes_(fragment.total_bytes_),
            saved_bytes_(fragment.saved_bytes_),
            fragment_(fragment.fragment_),
            packet_(std::move(fragment.packet_)),
            last_packet_(fragment.last_packet_)
        {

        }



        int IPFragment::Sniffer(void* stream, int size)
        {
            int rc = -EINVAL;

            // сюда попадаем только когда прочитан ip-заголовок фрагмента

            if (stream != nullptr && size > 0 )
            {



            }

            return rc;

        }


        int IPFragment::readFragment(void* stream, int size)
        {
            // сюда попадаем только с заголовком

            int rc = -EINVAL;

            if (stream != nullptr && size > 0)
            {
                // признак того, что пакет первый

                if (total_bytes_ < 0)
                {
                    packet_.Reset(data_.data());
                }

                // признак что пакет еще не вычитал даже заголовок

                if (packet_.Lost() < 0)
                {

                    headers::IP4HEADER &ip= *(headers::PIP4HEADER)stream;

                    unsigned short hlen = ip.hlen * 4;

                    unsigned short off_lo = ip.off_lo;
                    unsigned short off_hi = ip.off_hi;

                    unsigned short offset = ((off_lo << 3) | off_hi) * 8;

                    unsigned short len = (ip.length << 8) | (ip.length >> 8) - hlen;

                    int need_size = len + offset;

                    if (need_size > total_bytes_)
                    {
                        total_bytes_ = need_size;
                    }

                    if (total_bytes_ > data_.size())
                    {
                        data_.resize(total_bytes_);
                    }

                    last_packet_ |= (ip.f_M == 0);

                }

                rc = packet_.Sniffer(stream, size);

                if (rc >= 0)
                {
                    saved_bytes_ += rc;

                    if (packet_.IsComplete())
                    {

                    }
                }

            }

            return rc;
        }

        void IPFragment::Reset()
        {
            saved_bytes_ = 0;
            total_bytes_ = -1;
            last_packet_ = false;

            packet_.Reset(data_.data());

        }

        bool IPFragment::IsComplete() const
        {
            return last_packet_ && saved_bytes_ == total_bytes_;
        }

    }
}
