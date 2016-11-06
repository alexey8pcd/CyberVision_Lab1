#ifndef SIFTDESCRIPTORSSEARCHER_H
#define SIFTDESCRIPTORSSEARCHER_H
#include "descriptor.h"
#include "fimage.h"
#include "datatypes.h"
#include "imageutil.h"
#include "kernel.h"
#include "convolution.h"

const int SIFT_HISTOGRAM_SIZE = 8;
const int SIFT_DESCRIPTOR_SIZE = 4;
const int SIFT_CELL_SIZE = 4;
const int RI_HISTOGRAM_SIZE = 36;


constexpr float SIFT_DPHI = 3.1415 / SIFT_HISTOGRAM_SIZE;
constexpr float SIFT_HALF_DPHI = SIFT_DPHI / 2;
constexpr int SIFT_DESCRIPTOR_HALF_SIZE = SIFT_DESCRIPTOR_SIZE / 2;
constexpr int SIFT_DESC_SIZE = SIFT_DESCRIPTOR_SIZE
                               * SIFT_DESCRIPTOR_SIZE * SIFT_HISTOGRAM_SIZE;
constexpr int RI_DESC_SIZE = SIFT_DESCRIPTOR_SIZE
                             * SIFT_DESCRIPTOR_SIZE * RI_HISTOGRAM_SIZE;

enum GradientAlgorithm {
    SIMPLE,
    SOBEL
};

Descriptor<SIFT_DESC_SIZE> createSIFTDescriptor(
        const InterestPoint& point,
        const FImage& gradientLengthsStore,
        const FImage& gradientDirectionStore,
        EdgeType type);

Descriptor<RI_DESC_SIZE> createRIDescriptor(
        const InterestPoint& point, const FImage& gradientLengthsStore,
                const FImage& gradientDirectionStore, EdgeType type);

vector<Descriptor<RI_DESC_SIZE>> search(const FImage &image,
                                        const vector<InterestPoint> &points,
                                        EdgeType type,
                                        GradientAlgorithm algorithm,
                                        bool smoothed,
                                        double sigma = 1);

vector<Descriptor<SIFT_DESC_SIZE>> search(
        const FImage &image,
        const vector<InterestPoint> &points,
        EdgeType type,
        GradientAlgorithm algorithm,
        bool smoothed,
        double sigma = 1) ;

int calculateNumberOfNeighbornBasket(float halfBasketSize,
                                     float mod,
                                     int basketNumber);


#endif // SIFTDESCRIPTORSSEARCHER_H
