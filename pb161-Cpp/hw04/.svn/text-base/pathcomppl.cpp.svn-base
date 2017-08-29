#include "pathcomppl.h"

bool PathCompPL::betterThan(const PathInfo &path1, const PathInfo &path2)
{
    if (path1.getTotalPrice() < path2.getTotalPrice()) {
        return true;
    }
    if ((path1.getTotalPrice() == path2.getTotalPrice()) && (path1.getTotalLength() < path2.getTotalLength())) {
        return true;
    }
    return false;
}
