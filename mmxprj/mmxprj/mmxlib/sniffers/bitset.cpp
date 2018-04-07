#include "bitset.h"

#include <memory>

#include <cstring>

namespace mmx
{
    namespace sniffers
    {
        BitSet::BitSet(index_t bitsize) :
            set_((bitsize / 8) + 1)
        {

        }

        BitSet::BitSet(const BitSet& bitset) :
            set_(bitset.set_)
        {

        }

        BitSet::BitSet(BitSet&& bitset) :
            set_(std::move(bitset.set_))
        {

        }

        BitSet& BitSet::operator= (const BitSet& bitset)
        {
            set_ = bitset.set_;
            return *this;
        }

        BitSet& BitSet::operator= (BitSet&& bitset)
        {
            set_ = std::move(bitset.set_);
            return *this;
        }

        void BitSet::Set(index_t idx)
        {
            checkSize(idx);

            unsigned char &b = *(set_.data() + idx / 8);

            b |= 1 << idx % 8;

        }

        void BitSet::Clr(index_t idx)
        {
            checkSize(idx);

            unsigned char &b = *(set_.data() + idx / 8);

            b &= ~(1 << idx % 8);
        }

        bool BitSet::IsSet(index_t idx)
        {
            return idx < (set_.size() * 8) && (*(set_.data() + (idx / 8)) & (1 << idx % 8)) != 0;
        }

        bool BitSet::operator[](index_t idx)
        {
            return IsSet(idx);
        }

        void BitSet::Reset(int bitsize)
        {
            if (bitsize < 0)
            {
                bitsize = set_.size();
            }
            else
            {
                bitsize = bitsize / 8 + 1;

                if(bitsize > set_.size())
                {
                    bitsize = set_.size();
                }
            }

            std::memset(set_.data(), bitsize, 0);
        }

        void BitSet::checkSize(int idx)
        {
            if (idx >= (set_.size() * 8))
            {
                set_.resize((idx / 8) + 1);
            }

        }
    }
}
