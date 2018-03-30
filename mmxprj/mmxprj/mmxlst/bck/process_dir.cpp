#include "defines.h"
#include "direction.h"

#include <errno.h>

int process_dir(int dir_id, callback_routine call)
{
    int rc = -EINVAL;

    auto lambda = [](unsigned short port, mmx::staff::Packet&& packet, void* fn) {  ((callback_routine)(fn))(port, packet.Data(), packet.Size()); };

    mmxlst::Direction::GetDirection(dir_id).Process(lambda, (void*)call);

    return rc;
}

