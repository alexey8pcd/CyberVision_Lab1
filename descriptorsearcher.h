#ifndef DESCRIPTORSEARCHER_H
#define DESCRIPTORSEARCHER_H
#include "QVector"
#include "descriptor.h"
#include "datatypes.h"
#include "kernel.h"
#include "convolution.h"

class DescriptorSearcher {
    private:
        static Histogram createHistogram(int histogramSize, int descriptorRadius,
                   int x, int y, FImage &gradientLengthsStore,
                   FImage &gradientDirectionStore, EdgeType type);
        DescriptorSearcher();
    public:
        static QVector<QPair<Descriptor*, Descriptor*>*> associate(
                QVector<Descriptor*> descriptors1,
                QVector<Descriptor*> descriptors2);

        static QVector<Descriptor*> search(
                FImage &image, QVector<InterestPoint*> &points,
                EdgeType type, boolean smoothed, double sigma = 1);
};

#endif // DESCRIPTORSEARCHER_H
