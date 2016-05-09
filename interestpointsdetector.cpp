#include "interestpointsdetector.h"

InterestPointsDetector::InterestPointsDetector() {

}

InterestPointsDetector::InterestPointsDetector(
    FImage& image, EdgeType type, int radiusOfNeighborhood) {

    this->source = image;
    this->type = type;
    this->radiusOfNeighborhood = radiusOfNeighborhood;
}

QVector<InterestPointsDetector::InterestPoint*>*
        InterestPointsDetector::detectHarris(float threshold) {
    QVector<InterestPoint*>* points = new QVector<InterestPoint*>();

    return points;
}

QVector<InterestPointsDetector::InterestPoint*>*
        InterestPointsDetector::detectMoravec(float threshold) {
    int width = source.getWidth();
    int height = source.getHeight();
    FImage minValuesStore(width, height);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            float s = calculateOperatorValue(x, y);
            minValuesStore.setValue(x, y, s);
        }
    }

    QVector<InterestPoint*>* points = new QVector<InterestPoint*>();
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            float localMax = minValuesStore.getValue(x, y);
            for (int dpx = -radiusOfNeighborhood;
                 dpx <= radiusOfNeighborhood; ++dpx) {
                for (int dpy = -radiusOfNeighborhood;
                     dpy <= radiusOfNeighborhood; ++dpy) {
                    if(dpx == 0 && dpy == 0){
                        continue;
                    }
                    int xi = ImageUtil::handleEdgeEffect(x + dpx, width, type);
                    int yi = ImageUtil::handleEdgeEffect(y + dpy, height, type);
                    float neiValue = minValuesStore.getValue(xi, yi);
                    if (neiValue >= localMax){
                        goto label1;
                    }
                }
            }
            if(localMax > threshold) {
                points->push_back(new InterestPoint(x, y, localMax));
            }
            label1: ;
        }
    }
    return points;
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

