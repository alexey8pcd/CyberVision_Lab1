#include "descriptorssearcher.h"

vector<Descriptor<DESC_SIZE>> search(const FImage& image,
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

    FImage gradientLengthStore(image.getWidth(), image.getHeight());
    FImage gradientDirectionStore(image.getWidth(), image.getHeight());
    qDebug() << "Выполняю расчет модулей и углов градиента";
    for (int x = 0; x < image.getWidth(); ++x) {
        for (int y = 0; y < image.getHeight(); ++y) {
            float dx = dxStore.getValue(x, y);
            float dy = dyStore.getValue(x, y);
            float gradientLength = sqrtf(dx * dx + dy * dy);
            float phi = atan2(dy, dx) + M_PI;
            gradientLengthStore.setValue(x, y, gradientLength);
            gradientDirectionStore.setValue(x, y, phi);
        }
    }
    qDebug() << "Формирую дескрипторы";
    vector<Descriptor<DESC_SIZE>> descriptors
            = vector<Descriptor<DESC_SIZE>>();

    foreach (const InterestPoint& point, points) {

        Descriptor<DESC_SIZE> descriptor1
                = createDescriptor(point, gradientLengthStore,
                                   gradientDirectionStore, type);
        descriptors.push_back(descriptor1);
    }
    qDebug() << "Дескрипторы сформированы";
    return descriptors;
}

Descriptor<DESC_SIZE> createDescriptor(const InterestPoint& point,
        const FImage& gradientLengthStore,
        const FImage& gradientDirectionStore,
        EdgeType type) {

    Descriptor<DESC_SIZE> descriptor
            = Descriptor<DESC_SIZE>(point.x, point.y);
//    qDebug() << "Формирую дескриптор(ы) для точки: " << point.toQString();
    int histogramNumber = 0;
    int areaHalfSize = DESCRIPTOR_HALF_SIZE * CELL_SIZE;

    Kernel gaussKernel = Kernel::createGaussKernelByRadius(areaHalfSize);
    double cosP = cos(point.getOrientation());
    double sinP = sin(point.getOrientation());
    for (int xi = -areaHalfSize; xi < areaHalfSize; ++xi) {
        for (int yi = -areaHalfSize; yi < areaHalfSize; ++yi) {
            //повернутые смещения от центра интересной точки
            int dxr = (int)(xi * cosP - yi * sinP);
            int dyr = (int)(xi * sinP - yi * cosP);
            int rowIndex = dxr / CELL_SIZE;
            int columnIndex = dyr / CELL_SIZE;

            int x = ImageUtil::handleEdgeEffect(
                         point.x + xi, gradientLengthStore.getWidth(), type);
            int y = ImageUtil::handleEdgeEffect(
                         point.y + yi, gradientLengthStore.getHeight(), type);
            if(ImageUtil::insideImage(x, y)){
                float gradientPower = gaussKernel.getValue(xi, yi);

                float gradientLength = gradientPower *
                                       gradientLengthStore.getValue(x, y);
                float angle = gradientDirectionStore.getValue(x, y);
                if (angle < 0) {
                    angle = 2 * M_PI + angle;
                }

                //узнать номер текущей козины
                int basketNumber = (int)(angle / DESCRIPTOR_DPHI);
                float mod = angle - basketNumber * DESCRIPTOR_DPHI;

                //узнать номер соседней корзины
                int basketNumberNei = calculateNumberOfNeighbornBasket(
                                          DESCRIPTOR_HALF_DPHI, mod,
                                          basketNumber, HISTOGRAM_SIZE);
                //узнать расстояние до центра текущей корзины
                float centerOfBasket = basketNumber * DESCRIPTOR_DPHI
                                       + DESCRIPTOR_HALF_DPHI;
                float distanceToCenter = fabs(angle - fabs(centerOfBasket));
                float distanceToCenterNei = DESCRIPTOR_DPHI - distanceToCenter;
                float ratio = 1. - distanceToCenter / DESCRIPTOR_DPHI;
                float ratioNei = 1. - distanceToCenterNei / DESCRIPTOR_DPHI;
                int indexOffset = (rowIndex + DESCRIPTOR_HALF_SIZE) +
                                  (columnIndex + DESCRIPTOR_HALF_SIZE)
                                  * DESCRIPTOR_SIDE;
                descriptor.addValue(basketNumber + indexOffset,
                                    gradientLength * ratio);
                descriptor.addValue(basketNumberNei + indexOffset,
                                    gradientLength * ratioNei);
            }
        }
    }
    descriptor.normalize();
    descriptor.cutByThreshold(0.2f);
    descriptor.normalize();
    return descriptor;
}
