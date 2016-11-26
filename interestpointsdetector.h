#ifndef INTERESTPOINTSDETECTOR_H
#define INTERESTPOINTSDETECTOR_H
#include "fimage.h"
#include "imageutil.h"
#include "convolution.h"
#include "datatypes.h"
#include "vector"
#include "common.h"

using namespace std;
const int ROTATION_INV_BASKET_COUNT = 36;
constexpr float R_DPHI = 2 * M_PI / ROTATION_INV_BASKET_COUNT;
constexpr float R_HALF_DPHI = R_DPHI / 2;

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
        vector<InterestPoint> adaptiveNonMaximumSuppression(
                const vector<InterestPoint>& points);

        vector<InterestPoint> determinePointsByRadius(
                float threshold, int radius, const FImage &minValuesStore);

        vector<InterestPoint> calculateOrientations(const vector<InterestPoint>& points);

        vector<InterestPoint> filter(int radius,
            const vector<InterestPoint>& points);

        FImage calculateMinLambdasStore(const FImage& derXStore,
                                        const FImage& derYStore);

    public:
        InterestPointsDetector();
        void enableFilter(int maxPointsCount);
        void disableFilter();
        InterestPointsDetector(const FImage &image, EdgeType type,
                int radiusOfNeighborhood = 8);
        vector<InterestPoint> detectMoravec(float threshold = 0.);
        vector<InterestPoint> detectHarris(float threshold = 0.);
};
#endif // INTERESTPOINTSDETECTOR_H
