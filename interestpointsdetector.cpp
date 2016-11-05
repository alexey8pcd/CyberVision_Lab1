#include "interestpointsdetector.h"

InterestPointsDetector::InterestPointsDetector() {
    this->filterRequired = false;
    this->source = FImage();
    this->type = EdgeType_OUTSIDE_BLACK;
    this->radiusOfNeighborhood = 0;
}

void InterestPointsDetector::enableFilter(int maxPointsCount) {
    this->maxPointsCount = maxPointsCount;
    this->filterRequired = true;
}

void InterestPointsDetector::disableFilter() {
    this->filterRequired = false;
}

InterestPointsDetector::InterestPointsDetector(
        const FImage &image, EdgeType type, int radiusOfNeighborhood) {
    this->filterRequired = false;
    this->source = FImage(image);
    this->type = type;
    this->radiusOfNeighborhood = radiusOfNeighborhood;
}

FImage InterestPointsDetector::calculateMinLambdasStore(
        const FImage& derXStore, const FImage& derYStore)
{
    FImage minLambdasStore(source.getWidth(), source.getHeight());
    int areaSize = 2;
    for (int x = 0; x < source.getWidth(); ++x) {
        for (int y = 0; y < source.getHeight(); ++y) {
            float a = 0;
            float b = 0;
            float c = 0;
            for (int u = -areaSize; u <= areaSize; ++u) {
                for (int v = -areaSize; v <= areaSize; ++v) {
                    if (u == 0 && v == 0){
                        continue;
                    }
                    int xi = ImageUtil::handleEdgeEffect(
                                 x + u, source.getWidth(), type);
                    int yi = ImageUtil::handleEdgeEffect(
                                 y + v, source.getHeight(), type);
                    float ix = 0;
                    float iy = 0;
                    if(ImageUtil::insideImage(xi, yi)){
                         ix = derXStore.getValue(xi, yi);
                         iy = derYStore.getValue(xi, yi);
                    }
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
            minLambdasStore.setValue(x, y, lambdaMin);
        }
    }

    return minLambdasStore;
}

vector<InterestPoint> InterestPointsDetector::detectHarris(float threshold) {
    Kernel sobelX = Kernel::createSobelKernelX();
    Convolution convX = Convolution(sobelX, type);
    FImage derXStore = convX.apply(source);

    Kernel sobelY = Kernel::createSobelKernelY();
    Convolution convY = Convolution(sobelY, type);
    FImage derYStore = convY.apply(source);

    FImage minLambdasStore = calculateMinLambdasStore(derXStore, derYStore);
    vector<InterestPoint> points = determinePointsByRadius(
                             threshold, radiusOfNeighborhood, minLambdasStore);
    return adaptiveNonMaximumSuppression(points);
}

vector<InterestPoint> InterestPointsDetector::determinePointsByRadius(
        float threshold, int radius, const  FImage &minValuesStore)
{
    int width = minValuesStore.getWidth();
    int height = minValuesStore.getHeight();
    vector<InterestPoint> points = vector<InterestPoint>();
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            float localMax = minValuesStore.getValue(x, y);
            if(localMax < threshold) {
                continue;
            }

            boolean addPoint = true;
            for (int dpx = -radius; dpx <= radius; ++dpx) {
                for (int dpy = -radius; dpy <= radius && addPoint; ++dpy) {
                    if(dpx == 0 && dpy == 0){
                        continue;
                    }
                    int xi = ImageUtil::handleEdgeEffect(x + dpx, width, type);
                    int yi = ImageUtil::handleEdgeEffect(y + dpy, height, type);
                    float neiValue = 1e9;
                    if(ImageUtil::insideImage(xi, yi)){
                        neiValue = minValuesStore.getValue(xi, yi);
                    }
                    if (neiValue >= localMax){
                        addPoint = false;
                        break;
                    }

                }
            }
            if(addPoint){
                const InterestPoint point(x, y, localMax);
                points.push_back(point);
            }
        }
    }
    return points;
}

vector<InterestPoint> InterestPointsDetector::adaptiveNonMaximumSuppression(
        const vector<InterestPoint>& points) {
    int currentRadius = 1;
    vector<InterestPoint> sortedPoints(points);
    sort(sortedPoints.begin(),sortedPoints.end(),
         [](InterestPoint const &p1, InterestPoint const &p2){
        return p1.y < p2.y;
    });
    vector<InterestPoint> filtered
            = filter(currentRadius, sortedPoints);
    while(filtered.size() > maxPointsCount){
        filtered = filter(++currentRadius, sortedPoints);
    }

    return filtered;
}

///Правильная работа возможна только для сортированного по
/// возрастанию y-координаты вектора с интересными точками
vector<InterestPoint> InterestPointsDetector::filter(int radius,
    const vector<InterestPoint>& points){
    vector<InterestPoint> result = vector<InterestPoint>();

    for (int indexOfCenter = 0; indexOfCenter < points.size(); ++indexOfCenter) {
        const InterestPoint& center  = points.at(indexOfCenter);
        int yMin = center.y - radius;
        int yMax = center.y + radius;
        bool maxInLeftPart = true;
        for(int i = indexOfCenter; i >= 0; --i){
            const InterestPoint& p = points.at(i);
            if(p.y < yMin){
                break;
            }
            if (center.value < p.value && abs(p.x - center.x) <= radius ) {
                maxInLeftPart = false;
                break;
            }
        }
        bool maxInRightPart = true;
        for (int i = indexOfCenter; i < points.size(); ++i) {
            const InterestPoint& p = points.at(i);
            if(p.y > yMax){
                break;
            }
            if (center.value < p.value && abs(p.x - center.x) <= radius ) {
                maxInRightPart = false;
                break;
            }
        }
        if(maxInLeftPart && maxInRightPart){
            result.push_back(center);
        }
    }
    return result;
}

vector<InterestPoint> InterestPointsDetector::detectMoravec(float threshold) {
    int width = source.getWidth();
    int height = source.getHeight();
    FImage minValuesStore(width, height);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            float s = calculateOperatorValue(x, y);
            minValuesStore.setValue(x, y, s);
        }
    }
    const vector<InterestPoint>& points = determinePointsByRadius(
                             threshold, radiusOfNeighborhood, minValuesStore);
    return adaptiveNonMaximumSuppression(points);
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
    int areaSize = 1;
    for(int u = -areaSize; u <= areaSize; ++u) {
        for(int v = -areaSize; v <= areaSize; ++v) {
            int startX = ImageUtil::handleEdgeEffect(x + u, width, type);
            int startY = ImageUtil::handleEdgeEffect(y + v, height, type);
            int finishX = ImageUtil::handleEdgeEffect(x + u + dx, width, type);
            int finishY = ImageUtil::handleEdgeEffect(y + v + dy, height, type);
            if(ImageUtil::insideImage(startX, startY)
                && ImageUtil::insideImage(finishX, finishY)){
                float finishIntensity = source.getValue(finishX, finishY);
                float startIntensity = source.getValue(startX, startY);
                float diff = startIntensity - finishIntensity;
                c += diff * diff;
            }
        }
    }
    return c;
}

