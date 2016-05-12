#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "math.h"

class Histogram
{
    private:
        int basketsCount;
        float * baskets;
        float dphi;
        static const float PI_2 = 6.28318;
        void addValueToBasket(int index0, int index1, float value);
    public:
        void addValue(float phi, float value);
        void normalize();
        void normalize(float denominator);
        float getDistance(Histogram &histogram);
        float getModule();
        Histogram(int basketsCount);
        Histogram();
};

#endif // HISTOGRAM_H
