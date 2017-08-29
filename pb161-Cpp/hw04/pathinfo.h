#ifndef PATH_INFO
#define PATH_INFO

#include "edgeinfo.h"

class PathInfo {
    int totalLength;
    int totalPrice;
public:
    PathInfo();
    void addEdge(const EdgeInfo&);
    int getTotalLength() const;
    int getTotalPrice() const;
};

#endif
