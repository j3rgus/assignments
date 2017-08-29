#include "graph.h"
#include <iterator>

int Graph::addVertex(const std::string &vertex)
{
    graph.insert(std::make_pair(vertex, EDGE()));
    return 0;
}

int Graph::addEdge(const std::string& from, const std::string& to, int length, int price)
{
    if (graph.find(from) == graph.end())
        return 2;
    if (graph.find(to) == graph.end())
        return 2;
    if (graph[from].find(to) == graph[from].end())
        return 1;
    graph[from].insert(std::make_pair(to, EdgeInfo(length, price)));
    return 0;
}

int Graph::addBidirectionalEdge(const std::string& vertex1, const std::string& vertex2, int length, int price)
{
    if ((graph.find(vertex1) == graph.end()) || (graph.find(vertex2) == graph.end()))
        return 2;
    if (graph[vertex1].find(vertex2) == graph[vertex1].end())
        return 1;
    if (graph[vertex2].find(vertex1) == graph[vertex2].end())
        return 1;
    graph[vertex1].insert(std::make_pair(vertex2, EdgeInfo(length, price)));
    graph[vertex2].insert(std::make_pair(vertex1, EdgeInfo(length, price)));
    return 0;
}

int Graph::setEdgePrice(const std::string& from, const std::string& to, int price)
{
    if ((graph.find(from) == graph.end()))
        return 1;
    if (graph[from].find(to) == graph[from].end())
        return 1;
    graph[from][to].setPrice(price);
    return 0;
}

std::list<std::string> Graph::shortestPath(const std::string& from, const std::string& to, PathComp *pc) const
{

}
