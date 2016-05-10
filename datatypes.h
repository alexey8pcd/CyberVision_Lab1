#ifndef DATATYPES
#define DATATYPES

enum EdgeType {
    EdgeType_OUTSIDE_BLACK,
    EdgeType_COPY_EDGES,
    EdgeType_MIRROR_EDGES,
    EdgeType_WRAP_EDGES
};

struct InterestPoint{
    int x;
    int y;
    float value;
    InterestPoint(int x, int y, float value){
        this->x = x;
        this->y = y;
        this->value = value;
    }
};

#endif // DATATYPES


