#include "defines.h"
#include "direction.h"

int dispatch(int sock, int dir_id)
{
    return mmxlst::Direction::GetDirection(dir_id).Dispatch(sock);
}
