#include "defines.h"
#include "direction.h"

int create_dir(unsigned short begin, unsigned short end, int step)
{
    return mmxlst::Direction::BuildDirection(begin, end, step);
}
