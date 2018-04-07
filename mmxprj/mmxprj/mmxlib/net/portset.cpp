#include "portset.h"

#include <cstring>  // memset
#include <cstdlib>

namespace mmx
{
    namespace net
    {
        PortSet::PortSet()
        {
            Clear();
        }

        PortSet::PortSet(const PortSet& portset)
        {
            std::memcpy(set_, portset.set_, sizeof(set_));
        }

        PortSet::~PortSet()
        {

        }

        void PortSet::Set(unsigned short port)
        {
            set_[port / 8] =  set_[port / 8] | ((unsigned short)1 << (port % 8));
        }

        void PortSet::Clr(unsigned short port)
        {
            set_[port / 8] =  set_[port / 8] & ~((unsigned short)1 << (port % 8));
        }

        int PortSet::SetRange(unsigned short begin, unsigned short end, short step)
        {

            int rc = 0;

            step += (short)(step == 0);

            step = std::abs(step);

            step = begin <= end ? step : -step;

            // swap begin и end не прокатит, мы должны в люблом случае установить порт begin

            while (step > 0 ? begin <= end : begin >= end)
            {
                rc++;

                Set(begin);

                begin += step;
            }

            return rc;

        }

        int PortSet::ClrRange(unsigned short begin, unsigned short end, short step)
        {

            int rc = 0;

            step += (short)(step == 0);

            step = std::abs(step);

            step = begin <= end ? step : -step;

            // swap begin и end не прокатит, мы должны в люблом случае установить порт begin

            while (step > 0 ? begin <= end : begin >= end)
            {
                rc++;

                Clr(begin);

                begin += step;
            }

            return rc;

        }

        bool PortSet::operator[] (unsigned short port) const
        {
            return (set_[port / 8] & 1 << (port % 8)) != 0;
        }

        void PortSet::Clear()
        {
            std::memset(set_, 0, sizeof(set_));
        }
    }
}
