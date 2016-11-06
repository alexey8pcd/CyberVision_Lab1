#include "siftdescriptorssearcher.h"

vector<Descriptor<SIFT_DESC_SIZE>> search(const FImage& image,
                                const vector<InterestPoint>& points,
                                EdgeType type, GradientAlgorithm algorithm,
                                          bool smoothed, double sigma)
{


    qDebug() << "Выполняю поиск дескрипторов";
    FImage tmp = FImage(image);
    if(smoothed){
        qDebug() << "Сглаживание включено";
        Kernel gaussXKernel = Kernel::createGaussSeparateKernelX(sigma);
        Kernel gaussYKernel = Kernel::createGaussSeparateKernelY(sigma);

        Convolution convX = Convolution(gaussXKernel, type);
        Convolution convY = Convolution(gaussYKernel, type);
        qDebug() << "Выполняю сглаживание";
        FImage imageResultX = convX.apply(image);
        tmp = convY.apply(imageResultX);
        tmp.normalize();
        qDebug() << "Сглаживание завершено";

    }
    Kernel gradX;
    Kernel gradY;
    if(algorithm == GradientAlgorithm::SIMPLE){
        gradX = Kernel::createSimpleGradientKernelX();
        gradY = Kernel::createSimpleGradientKernelY();
    } else {
        gradX = Kernel::createSobelKernelX();
        gradY = Kernel::createSobelKernelY();
    }
    qDebug() << "Нахожу компоненты градиента";
    Convolution convX = Convolution(gradX, type);
    Convolution convY = Convolution(gradY, type);
    FImage dxStore = convX.apply(tmp);
    FImage dyStore = convY.apply(tmp);

    FImage gradientLengthsStore(image.getWidth(), image.getHeight());
    FImage gradientDirectionsStore(image.getWidth(), image.getHeight());
    qDebug() << "Выполняю расчет модулей и углов градиента";
    for (int x = 0; x < image.getWidth(); ++x) {
        for (int y = 0; y < image.getHeight(); ++y) {
            float dx = dxStore.getValue(x, y);
            float dy = dyStore.getValue(x, y);
            float gradientLength = sqrtf(dx * dx + dy * dy);
            float phi = atan2(dy, dx) + M_PI;
            gradientLengthsStore.setValue(x, y, gradientLength);
            gradientDirectionsStore.setValue(x, y, phi);
        }
    }
    qDebug() << "Формирую дескрипторы";
    vector<Descriptor<SIFT_DESC_SIZE>> descriptors
            = vector<Descriptor<SIFT_DESC_SIZE>>();

    foreach (const InterestPoint& point, points) {
        Descriptor<SIFT_DESC_SIZE> descriptor
                = createSIFTDescriptor(point,
                                   gradientLengthsStore,
                                   gradientDirectionsStore,
                                   type);
        descriptors.push_back(descriptor);
    }
    qDebug() << "Дескрипторы сформированы";
    return descriptors;
}

Descriptor<SIFT_DESC_SIZE> createSIFTDescriptor(const InterestPoint& point,
        const FImage& gradientLengthsStore,
        const FImage& gradientDirectionStore,
        EdgeType type)
{
    Descriptor<SIFT_DESC_SIZE> descriptor
            = Descriptor<SIFT_DESC_SIZE>(point.x, point.y);
    int histogramNumber = 0;
    float kernelRadius = SIFT_DESCRIPTOR_HALF_SIZE * SIFT_CELL_SIZE;
    Kernel gaussKernel = Kernel::createGaussKernelByRadius(kernelRadius);


    for (int histogramRowIndex = -SIFT_DESCRIPTOR_HALF_SIZE;
         histogramRowIndex < SIFT_DESCRIPTOR_HALF_SIZE; ++histogramRowIndex) {
        for (int histogramColumnIndex = -SIFT_DESCRIPTOR_HALF_SIZE;
             histogramColumnIndex < SIFT_DESCRIPTOR_HALF_SIZE; ++histogramColumnIndex) {
            int indexOffset = histogramNumber *  SIFT_HISTOGRAM_SIZE;
            int x = point.x + histogramRowIndex * SIFT_CELL_SIZE;
            int y = point.y + histogramColumnIndex * SIFT_CELL_SIZE;

            const int mid = SIFT_CELL_SIZE / 2;
            for (int xx = x, i = 0; i <= SIFT_CELL_SIZE; ++xx, ++i) {
                for (int yy = y, j = 0; j <= SIFT_CELL_SIZE; ++yy, ++j) {
                    if(xx == x + mid && yy == y + mid){
                        continue;
                    }
                    int xi = ImageUtil::handleEdgeEffect(
                                 xx, gradientLengthsStore.getWidth(), type);
                    int yi = ImageUtil::handleEdgeEffect(
                                 yy, gradientLengthsStore.getHeight(), type);
                    if(ImageUtil::insideImage(xi, yi)){
                        int gki = histogramRowIndex * SIFT_CELL_SIZE + i;
                        int gkj = histogramColumnIndex * SIFT_CELL_SIZE + j;

                        float gradPower = gaussKernel.getValue(gki, gkj);

                        float gradientLength = gradPower *
                                               gradientLengthsStore.getValue(xi, yi);
                        float angle = gradientDirectionStore.getValue(xi, yi);
                        if (angle < 0) {
                            angle = 2 * M_PI + angle;
                        }

                        //узнать номер текущей козины
                        int basketNumber = (int)(angle / SIFT_DPHI);
                        float mod = angle - basketNumber * SIFT_DPHI;

                        //узнать номер соседней корзины
                        int basketNumberNei = calculateNumberOfNeighbornBasket(
                                                  SIFT_HALF_DPHI, mod, basketNumber);
                        //узнать расстояние до центра текущей корзины
                        float centerOfBasket = basketNumber * SIFT_DPHI + SIFT_HALF_DPHI;
                        float distanceToCenter = fabs(angle - fabs(centerOfBasket));
                        float distanceToCenterNei = SIFT_DPHI - distanceToCenter;
                        float ratio = 1. - distanceToCenter / SIFT_DPHI;
                        float ratioNei = 1. - distanceToCenterNei / SIFT_DPHI;

                        descriptor.addValue(basketNumber + indexOffset,
                                            gradientLength * ratio);
                        descriptor.addValue(basketNumberNei + indexOffset,
                                            gradientLength * ratioNei);
                    }
                }
            }
            descriptor.normalizeHistogram(SIFT_HISTOGRAM_SIZE, histogramNumber++);
        }
    }
    descriptor.normalize();
    descriptor.cutByThreshold(0.2f);
    descriptor.normalize();
    return descriptor;
}

int calculateNumberOfNeighbornBasket(
        float halfBasketSize, float mod, int basketNumber)
{
    int basketNumberNei;
    if(mod >= halfBasketSize) {
        basketNumberNei = basketNumber + 1;
    } else {
        basketNumberNei = basketNumber - 1;
    }
    if(basketNumberNei == SIFT_HISTOGRAM_SIZE){
        basketNumberNei = 0;
    }
    if(basketNumberNei < 0){
        basketNumberNei = SIFT_HISTOGRAM_SIZE - 1;
    }
    return basketNumberNei;
}

vector<Descriptor<RI_DESC_SIZE> > search(const FImage& image,
                                         const vector<InterestPoint>& points,
                                         EdgeType type,
                                         GradientAlgorithm algorithm,
                                         bool smoothed, double sigma) {

}


Descriptor<RI_DESC_SIZE> createRIDescriptor(
        const InterestPoint& point, const FImage& gradientLengthsStore,
                const FImage& gradientDirectionStore, EdgeType type){

}

