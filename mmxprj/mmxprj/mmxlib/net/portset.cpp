#include "portset.h"

#include <cstring>  // memset

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

        bool PortSet::operator[] (unsigned short port)
        {
            return (set_[port / 8] & 1 << (port % 8)) != 0;
        }

        void PortSet::Clear()
        {
            std::memset(set_, 0, sizeof(set_));
        }
    }
}
