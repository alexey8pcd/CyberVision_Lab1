#include "interestpointsdetector.h"

InterestPointsDetector::InterestPointsDetector() {

}

void InterestPointsDetector::enableFilter(int maxPointsCount) {
    this->maxPointsCount = maxPointsCount;
    this->filterRequired = true;
}

void InterestPointsDetector::disableFilter() {
    this->filterRequired = false;
}

InterestPointsDetector::InterestPointsDetector(
        FImage &image, EdgeType type, int radiusOfNeighborhood) {
    this->filterRequired = false;
    this->source = image;
    this->type = type;
    this->radiusOfNeighborhood = radiusOfNeighborhood;
}

QVector<InterestPoint*> InterestPointsDetector::detectHarris(float threshold) {
    float values[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    Kernel sobelX(values, 3);
    FImage derXStore = Convolution::apply(source, sobelX, type);

    float sobelYValues[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    Kernel sobelY(sobelYValues, 3);
    FImage derYStore = Convolution::apply(source, sobelY, type);

    FImage minValuesStore(source.getWidth(), source.getHeight());
    for (int x = 0; x < source.getWidth(); ++x) {
        for (int y = 0; y < source.getHeight(); ++y) {
            float a = 0;
            float b = 0;
            float c = 0;
            for (int u = -1; u <= 1; ++u) {
                for (int v = -1; v <= 1; ++v) {
                    if (u == 0 && v == 0){
                        continue;
                    }
                    int xi = ImageUtil::handleEdgeEffect(
                                 x + u, source.getWidth(), type);
                    int yi = ImageUtil::handleEdgeEffect(
                                 y + v, source.getHeight(), type);
                    float ix = derXStore.getValue(xi, yi);
                    float iy = derYStore.getValue(xi, yi);
                    a += ix * ix;
                    b += ix * iy;
                    c += iy * iy;
                }
            }
            float trA = a + c;
            float detA = a * c - b * b;
            float rad = sqrtf(trA * trA - 4 * detA);
            float lambda1 = (trA - rad) / 2;
            float lambda2 = (trA + rad) / 2;
            float lambdaMin = lambda1 < lambda2 ? lambda1 : lambda2;
            minValuesStore.setValue(x, y, lambdaMin);
        }
    }
    return determineInterestPoints(minValuesStore, threshold);
}

QVector<InterestPoint*> InterestPointsDetector::determinePointsByRadius(
        float threshold, int radius, FImage &minValuesStore)
{
    int width = minValuesStore.getWidth();
    int height = minValuesStore.getHeight();

    QVector<InterestPoint*> points = QVector<InterestPoint*>();
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            float localMax = minValuesStore.getValue(x, y);
            if(localMax < threshold) {
                continue;
            }
            for (int dpx = -radius; dpx <= radius; ++dpx) {
                for (int dpy = -radius; dpy <= radius; ++dpy) {
                    if(dpx == 0 && dpy == 0){
                        continue;
                    }
                    int xi = ImageUtil::handleEdgeEffect(x + dpx, width, type);
                    int yi = ImageUtil::handleEdgeEffect(y + dpy, height, type);
                    float neiValue = minValuesStore.getValue(xi, yi);
                    if (neiValue >= localMax){
                        goto endFor;
                    }
                }
            }
            points.push_back(new InterestPoint(x, y, localMax));
            endFor: ;
        }
    }
    return points;
}

QVector<InterestPoint*> InterestPointsDetector::determineInterestPoints(
        FImage &minValuesStore, float threshold) {
    QVector<InterestPoint*> points = determinePointsByRadius(
                             threshold, radiusOfNeighborhood, minValuesStore);
    if(filterRequired){
        if (points.size() > maxPointsCount) {
            int radius = radiusOfNeighborhood + 1;
            while (points.size() > maxPointsCount) {
                points = determinePointsByRadius(
                             threshold, radius++, minValuesStore);
            }
        }
    }
    return points;
}

QVector<InterestPoint*> InterestPointsDetector::detectMoravec(float threshold) {
    int width = source.getWidth();
    int height = source.getHeight();
    FImage minValuesStore(width, height);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            float s = calculateOperatorValue(x, y);
            minValuesStore.setValue(x, y, s);
        }
    }
    return determineInterestPoints(minValuesStore, threshold);
}

float InterestPointsDetector::calculateOperatorValue(int x, int y) {
    float minContrastValue = 1e6;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            float contrastValue = calculateContrastForShift(x, y, dx, dy);
            if(contrastValue < minContrastValue){
                minContrastValue = contrastValue;
            }
        }
    }
    return minContrastValue;
}

float InterestPointsDetector::calculateContrastForShift(
        int x, int y, int dx, int dy) {
    float c = 0;
    int width = source.getWidth();
    int height = source.getHeight();
    for(int u = -1; u <= 1; ++u) {
        for(int v = -1; v <= 1; ++v) {
            int startX = ImageUtil::handleEdgeEffect(x + u, width, type);
            int startY = ImageUtil::handleEdgeEffect(y + v, height, type);
            int finishX = ImageUtil::handleEdgeEffect(x + u + dx, width, type);
            int finishY = ImageUtil::handleEdgeEffect(y + v + dy, height, type);
            float finishIntensity = source.getValue(finishX, finishY);
            float startIntensity = source.getValue(startX, startY);
            float diff = startIntensity - finishIntensity;
            c += diff * diff;
        }
    }
    return c;
}

