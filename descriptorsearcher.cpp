#include "descriptorsearcher.h"

Histogram DescriptorSearcher::createHistogram(int histogramSize,
                                              int cellSize,
              int left, int top, FImage &gradientLengthsStore,
              FImage &gradientDirectionStore, EdgeType type)
{
    Histogram histogram(histogramSize);
    int mid = cellSize / 2;
    for (int i = 0; i <= cellSize; ++i) {
        for (int j = 0; j <= cellSize; ++j) {
            if(i == mid && j == mid){
                continue;
            }
            int xi = ImageUtil::handleEdgeEffect(
                         left + i, gradientLengthsStore.getWidth(), type);
            int yi = ImageUtil::handleEdgeEffect(
                         top + j, gradientLengthsStore.getHeight(), type);
            float gradientLength = gradientLengthsStore.getValue(xi, yi);
            float phi = gradientDirectionStore.getValue(xi, yi);
            histogram.addValue(phi, gradientLength);
        }
    }
    histogram.normalize();
    return histogram;
}

QVector<Descriptor*> DescriptorSearcher::search(
        FImage &image,
        QVector<InterestPoint*> &points,
        EdgeType type, boolean smoothed, double sigma) {
    Kernel sobelX = Kernel::createSobelKernelX();
    Kernel sobelY = Kernel::createSobelKernelY();

    if(smoothed){
        Kernel gaussXKernel = Kernel::createGaussSeparateKernelX(sigma);
        FImage resultX = Convolution::apply(image, gaussXKernel, type);
        Kernel gaussYKernel = Kernel::createGaussSeparateKernelY(sigma);
        FImage resultY = Convolution::apply(resultX, gaussYKernel, type);
        image = resultY;
    }
    FImage dxStore = Convolution::apply(image, sobelX, type);
    FImage dyStore = Convolution::apply(image, sobelY, type);
    FImage gradientLengthsStore(image.getWidth(), image.getHeight());
    FImage gradientDirectionsStore(image.getWidth(), image.getHeight());
    for (int x = 0; x < image.getWidth(); ++x) {
        for (int y = 0; y < image.getHeight(); ++y) {
            float dx = dxStore.getValue(x, y);
            float dy = dyStore.getValue(x, y);
            float gradientLength = sqrtf(dx * dx + dy * dy);
            float phi = atan2(dy, dx);
            gradientLengthsStore.setValue(x, y, gradientLength);
            gradientDirectionsStore.setValue(x, y, phi);
        }
    }

    QVector<Descriptor*> descriptors = QVector<Descriptor*>();
    int descriptorRadius = 4;
    int cellSize = 4;
    int halfDescRadius = descriptorRadius / 2;
    int descRaduis2 = 16;
    int histogramSize = 8;
    foreach (InterestPoint* point, points) {
        Descriptor * descriptor = new Descriptor(
                          point->x, point->y, descRaduis2);
        int index = 0;
        for (int i = -halfDescRadius; i < halfDescRadius; ++i) {
            for (int j = -halfDescRadius; j < halfDescRadius; ++j) {
                int x = point->x + i * cellSize;
                int y = point->y + j * cellSize;
                Histogram histogram = createHistogram(histogramSize,
                       cellSize, x, y, gradientLengthsStore,
                           gradientDirectionsStore, type);
                descriptor->setHistogram(index++, histogram);
            }
        }
        descriptor->normalize();
        descriptors.push_back(descriptor);
    }
    return descriptors;
}

DescriptorSearcher::DescriptorSearcher() {

}

QVector<QPair<Descriptor*, Descriptor*>*>
    DescriptorSearcher::associate(
        QVector<Descriptor*> descriptors1,
        QVector<Descriptor*> descriptors2) {
    QVector<QPair<Descriptor*, Descriptor*>*> pairs =
            QVector<QPair<Descriptor*, Descriptor*>*>();

    if(!descriptors1.isEmpty() && !descriptors2.isEmpty()){
        for (int i = 0; i < descriptors1.size(); ) {
            if(descriptors2.isEmpty()){
                break;
            }
            Descriptor * descriptor = descriptors1.at(i);
            int nearestIndex = 0;
            Descriptor * nearest = descriptors2.at(nearestIndex);
            float min = descriptor->getDistance(*nearest);
            for (int j = 1; j < descriptors2.size(); ++j) {
                Descriptor * descriptor2 = descriptors2.at(j);
                float distance = descriptor->getDistance(*descriptor2);
                if(distance < min){
                    nearest = descriptor2;
                    min = distance;
                    nearestIndex = j;
                }
            }
            pairs.push_back(
                  new QPair<Descriptor*, Descriptor*>(descriptor, nearest));
//            descriptors2.removeAt(nearestIndex);
            descriptors1.removeAt(i);

        }
    }
    return pairs;
}

