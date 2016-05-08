#ifndef CONVULSION_H
#define CONVULSION_H

#include "qimage.h"
#include "kernel.h"
#include "math.h"
#include "imageutil.h"
#include "edgetype.h"
#include "fimage.h"
class Convolution
{
    public:
        Convolution();
        static FImage apply(FImage &image, Kernel &kernel,
                            EdgeType edgesType);
//        static QImage applyGauss(const QImage &image, const double sigma);

    private:
        static float applyToPixel(FImage &image, int x, int y,
                           Kernel &kernel, EdgeType edgesType);
};

#endif // CONVULSION_H
