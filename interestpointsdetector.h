#ifndef INTERESTPOINTSDETECTOR_H
#define INTERESTPOINTSDETECTOR_H
#include "fimage.h"
#include "imageutil.h"
#include "convolution.h"
#include "datatypes.h"
class InterestPointsDetector
{
    private:
        FImage source;
        EdgeType type;
        int radiusOfNeighborhood;
        int maxPointsCount;
        bool filterRequired;
        float calculateOperatorValue(int x, int y);
        float calculateContrastForShift(int x, int y, int dx, int dy);
        QVector<InterestPoint*> determineInterestPoints(
                FImage &minValuesStore, float threshold);

        QVector<InterestPoint*> determinePointsByRadius(
                float threshold, int radius, FImage &minValuesStore);

    public:
        InterestPointsDetector();
        void enableFilter(int maxPointsCount);
        void disableFilter();
        InterestPointsDetector(FImage &image, EdgeType type,
                int radiusOfNeighborhood = 1);
        QVector<InterestPoint*> detectMoravec(float threshold = 0.);
        QVector<InterestPoint*> detectHarris(float threshold = 0.);
};
#endif // INTERESTPOINTSDETECTOR_H
