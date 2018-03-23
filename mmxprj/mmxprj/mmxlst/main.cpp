#include "mmxlib/logs/log.h"
#include "defines.h"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG
//#define DEFAULT_

int main(int argc, char* argv[])
{
    mmx::logs::log_init("/tmp/mmx_lst", DEFAULT_LEVEL_LOG, false);

    int rc = listener(0,5000,5002,1);

    return rc;
}

