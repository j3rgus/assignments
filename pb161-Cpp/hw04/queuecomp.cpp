#include "queuecomp.h"

QueueComp::QueueComp(PathComp *p)
{
    pc = p;
}

bool QueueComp::operator() (const pair<PathInfo, string>& item1, const pair<PathInfo, string>& item2)
{
    return pc->betterThan(item2.first, item1.first);
}
