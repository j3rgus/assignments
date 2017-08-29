#ifndef QUEUE_COMP
#define QUEUE_COMP

#include <string>
#include "pathcomp.h"
#include "pathinfo.h"

using std::pair;
using std::string;

class QueueComp {
    PathComp *pc;
public:
    QueueComp(PathComp *);
    bool operator() (const pair<PathInfo, string>& item1, const pair<PathInfo, string>& item2);
};

#endif
