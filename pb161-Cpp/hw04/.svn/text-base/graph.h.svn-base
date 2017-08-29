#ifndef GRAPH
#define GRAPH

#include <string>
#include <list>
#include <map>
#include "pathcomp.h"

class Graph {
//    struct edge_info {
//        EdgeInfo edgeInfo;
//        bool edge;
//        edge_info(int i, int j) {edge}
//    };
    typedef std::map<std::string, EdgeInfo> EDGE;
    std::map<std::string, EDGE > graph;
public:
    int addVertex(const std::string&);
    int addEdge(const std::string&, const std::string&, int, int);
    int addBidirectionalEdge(const std::string&, const std::string&, int, int);
    int setEdgePrice(const std::string&, const std::string&, int);
    std::list<std::string> shortestPath(const std::string&, const std::string&, PathComp*) const;
};

#endif
