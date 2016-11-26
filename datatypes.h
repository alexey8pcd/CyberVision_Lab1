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
    double orientation;

    InterestPoint(int x, int y, float value){
        this->x = x;
        this->y = y;
        this->value = value;
        this->orientation = 0;
    }

    void setOrientation(double value){
        this->orientation = value;
    }

    double getOrientation() const {
        return this->orientation;
    }

    QString toQString() const {
        QString s = QString::number(x);
        s.append(";").append(QString::number(y)).
                append(";").append(QString::number(orientation));
        return s;
    }
};

#endif // DATATYPES


