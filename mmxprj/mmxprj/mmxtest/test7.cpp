#include "mmxlib/headers/order645_2.h"
#include <cstring>

namespace mmxtest
{
    int test7()
    {
        mmx::headers::ORDER_645_2_HEADER header;
        char dummy_;

        char* p = (char*)&header;

        std::memset(&header, 0, sizeof(header));

        p[10] = 4;

        *(unsigned int*)&p[11] = 123456;

        p[1] = p[1];

        return 0;

    }
}
