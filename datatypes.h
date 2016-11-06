#ifndef DATATYPES
#define DATATYPES
#include "QString"

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
    QString toQString() const {
        QString s = QString::number(x);
        s.append(";").append(QString::number(y));
        return s;
    }
};

#endif // DATATYPES


