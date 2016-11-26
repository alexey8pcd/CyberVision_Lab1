#ifndef SIFTDESCRIPTORSSEARCHER_H
#define SIFTDESCRIPTORSSEARCHER_H
#include "descriptor.h"
#include "fimage.h"
#include "datatypes.h"
#include "imageutil.h"
#include "kernel.h"
#include "convolution.h"
#include "common.h"

const int HISTOGRAM_SIZE = 8;
const int DESCRIPTOR_SIDE = 4;
const int CELL_SIZE = 4;

constexpr float DESCRIPTOR_DPHI = 3.1415 / HISTOGRAM_SIZE;
constexpr float DESCRIPTOR_HALF_DPHI = DESCRIPTOR_DPHI / 2;
constexpr int DESCRIPTOR_HALF_SIZE = DESCRIPTOR_SIDE / 2;
constexpr int DESC_SIZE = DESCRIPTOR_SIDE
                               * DESCRIPTOR_SIDE * HISTOGRAM_SIZE;

enum GradientAlgorithm {
    SIMPLE,
    SOBEL
};

Descriptor<DESC_SIZE> createDescriptor(
        const InterestPoint& point,
        const FImage& gradientLengthsStore,
        const FImage& gradientDirectionStore,
        EdgeType type);

vector<Descriptor<DESC_SIZE>> search(
        const FImage &image,
        const vector<InterestPoint> &points,
        EdgeType type,
        GradientAlgorithm algorithm,
        bool smoothed,
        double sigma = 1) ;





#endif // SIFTDESCRIPTORSSEARCHER_H
