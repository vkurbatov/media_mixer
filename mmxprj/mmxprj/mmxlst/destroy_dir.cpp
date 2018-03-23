#include "defines.h"
#include "direction.h"

int destroy_dir(int dir_id)
{
    return mmxlst::Direction::DestroyDirection(dir_id);
}
