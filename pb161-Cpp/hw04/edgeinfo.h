#ifndef EDGE_INFO
#define EDGE_INFO

class EdgeInfo {
    int length;
    int price;
public:
    EdgeInfo();
    EdgeInfo(int, int);
    void setPrice(int);
    int getLength() const;
    int getPrice() const;
};

#endif
