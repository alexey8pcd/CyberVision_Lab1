#ifndef INTERESTPOINTSDETECTOR_H
#define INTERESTPOINTSDETECTOR_H
#include "fimage.h"
#include "imageutil.h"
class InterestPointsDetector
{
    private:
        FImage source;
        EdgeType type;
        int radiusOfNeighborhood;
        float calculateOperatorValue(int x, int y);

    public:
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

        InterestPointsDetector();
        InterestPointsDetector(FImage &image, EdgeType type,
                int radiusOfNeighborhood = 1);
        QVector<InterestPoint*>* detectMoravec(float threshold = 0.);
        QVector<InterestPoint*>* detectHarris(float threshold = 0.);
        float calculateContrastForShift(int x, int y, int dx, int dy);
};
#endif // INTERESTPOINTSDETECTOR_H
