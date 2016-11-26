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

    vector<InterestPoint> mostPowerPoint = adaptiveNonMaximumSuppression(points);
    vector<InterestPoint> mostPPWithOrientation = calculateOrientations(mostPowerPoint);
    return mostPPWithOrientation;
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
                InterestPoint point(x, y, localMax);
                points.push_back(point);
            }
        }
    }
    return points;
}

vector<InterestPoint> InterestPointsDetector::calculateOrientations(
        const vector<InterestPoint>& points) {
    Kernel gaussKernel = Kernel::createGaussKernelByRadius(radiusOfNeighborhood);

    vector<InterestPoint> result = vector<InterestPoint>();
    foreach (InterestPoint point, points) {
        float rBaskets[ROTATION_INV_BASKET_COUNT] = {0.f};
        //заполнить корзины
        for (int dx = -radiusOfNeighborhood; dx <= radiusOfNeighborhood; ++dx) {
            for (int dy = -radiusOfNeighborhood; dy <= radiusOfNeighborhood; ++dy) {
                int x1 = ImageUtil::handleEdgeEffect(dx + point.x + 1, source.getWidth(), type);
                int x2 = ImageUtil::handleEdgeEffect(dx + point.x - 1, source.getWidth(), type);
                int y1 = ImageUtil::handleEdgeEffect(dy + point.y + 1, source.getHeight(), type);
                int y2 = ImageUtil::handleEdgeEffect(dy + point.y - 1, source.getHeight(), type);
                int x0 = ImageUtil::handleEdgeEffect(dx + point.x, source.getWidth(), type);
                int y0 = ImageUtil::handleEdgeEffect(dy + point.y, source.getHeight(), type);
                if(ImageUtil::insideImage(x1, y0)
                        && ImageUtil::insideImage(x2, y0)
                        && ImageUtil::insideImage(x0, y1)
                        && ImageUtil::insideImage(x0, y2)){

                    float lx1y0 = source.getValue(x1, y0);
                    float lx2y0 = source.getValue(x2, y0);

                    float lx0y1 = source.getValue(x0, y1);
                    float lx0y2 = source.getValue(x0, y2);

                    float dlx = lx1y0 - lx2y0;
                    float dly = lx0y1 - lx0y2;
                    float gradientLength = sqrtf(dlx * dlx + dly * dly);
                    float angle = atan2(dly, dlx) + M_PI;
                    gradientLength *= gaussKernel.getValue(dx, dy);

                    //узнать номер текущей козины
                    int basketNumber = (int)(angle / R_DPHI);
                    float mod = angle - basketNumber * R_DPHI;

                    //узнать номер соседней корзины
                    int basketNumberNei = calculateNumberOfNeighbornBasket(
                                              R_HALF_DPHI, mod,
                                              basketNumber, ROTATION_INV_BASKET_COUNT);
                    //узнать расстояние до центра текущей корзины
                    float centerOfBasket = basketNumber * R_DPHI + R_HALF_DPHI;
                    float distanceToCenter = fabs(angle - fabs(centerOfBasket));
                    float distanceToCenterNei = R_DPHI - distanceToCenter;
                    float ratio = 1. - distanceToCenter / R_DPHI;
                    float ratioNei = 1. - distanceToCenterNei / R_DPHI;
                    rBaskets[basketNumber] += gradientLength * ratio;
                    rBaskets[basketNumberNei] += gradientLength * ratioNei;
                }

            }
        }
        //найти пики
        int peakIndex = 0;
        float peakValue = rBaskets[peakIndex];
        for (int i = 1; i < ROTATION_INV_BASKET_COUNT; ++i) {
            if (rBaskets[i] > peakValue) {
                peakValue = rBaskets[i];
                peakIndex = i;
            }
        }
        int xL, xR;
        if (peakIndex == 0){
            xL = ROTATION_INV_BASKET_COUNT - 1;
        } else {
            xL = peakIndex - 1;
        }
        if (peakIndex == ROTATION_INV_BASKET_COUNT - 1){
            xR = 0;
        } else {
            xR = peakIndex + 1;
        }
        double angleOfPeak = maxInterp3f(rBaskets[xL], peakValue, rBaskets[xR]);
        point.setOrientation(angleOfPeak);
        result.push_back(point);

        int subPeakIndex = -1;
        float subPeakValue = 0.8 * peakValue;
        for (int i = 0; i < ROTATION_INV_BASKET_COUNT; ++i) {
            if (peakIndex != i && rBaskets[i] > subPeakValue) {
                subPeakValue = rBaskets[i];
                subPeakIndex = i;
            }
        }
        if (subPeakIndex != -1) {
            if (subPeakIndex == 0){
                xL = ROTATION_INV_BASKET_COUNT - 1;
            } else {
                xL = subPeakIndex - 1;
            }
            if (subPeakIndex == ROTATION_INV_BASKET_COUNT - 1){
                xR = 0;
            } else {
                xR = subPeakIndex + 1;
            }
            double angleOfSubPeak = maxInterp3f(
                                        rBaskets[xL], subPeakValue, rBaskets[xR]);
            InterestPoint copy = InterestPoint(point.x, point.y, point.value);
            copy.setOrientation(angleOfSubPeak);
            result.push_back(copy);
        }
    }
    return result;
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
        for(int i = indexOfCenter; i >= 0; --i) {
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

