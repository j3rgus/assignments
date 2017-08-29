#include "pathinfo.h"

PathInfo::PathInfo(): totalLength(0), totalPrice(0)
{
}

int PathInfo::getTotalLength() const
{
    return totalLength;
}

int PathInfo::getTotalPrice() const
{
    return totalPrice;
}

void PathInfo::addEdge(const EdgeInfo &edge)
{
    totalLength += edge.getLength();
    totalPrice += edge.getPrice();
}
