#ifndef _MMX_SNIFFERS_BITSET_H
#define _MMX_SNIFFERS_BITSET_H

#include <vector>

namespace mmx
{
    namespace sniffers
    {
        static const int DEFAULT_MAX_INDEX = 4096;

        using index_t = unsigned short ;

        class BitSet
        {
            std::vector<unsigned char> set_;

        public:
            BitSet(index_t bitsize);
            BitSet(const BitSet& bitset);
            BitSet(BitSet&& bitset);
            BitSet& operator= (const BitSet& bitset);
            BitSet& operator= (BitSet&& bitset);
            ~BitSet();
            void Set(index_t idx);
            void Clr(index_t idx);
            bool IsSet(index_t idx);
            bool operator[](index_t idx);
            void Reset(int bitsize = -1);
        private:
            void checkSize(int idx);
        };
    }
}

#endif
