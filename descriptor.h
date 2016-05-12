#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include "histogram.h"

class Descriptor {

    private:
        int histogramCount;
        Histogram * histograms;
        int x;
        int y;
    public:
        Descriptor(int x, int y, int histogramCount);
        Descriptor();
        void setHistogram(int index, Histogram &histogram);
        float getModule();
        float getDistance(Descriptor &descriptor);
        void normalize();
        int getX();
        int getY();
};

#endif // DESCRIPTOR_H
