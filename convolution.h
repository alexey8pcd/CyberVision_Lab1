#ifndef CONVULSION_H
#define CONVULSION_H

#include "qimage.h"
#include "kernel.h"
#include "math.h"
#include "imageutil.h"
#include "fimage.h"

class Convolution
{
    public:
        Convolution(Kernel &kernel, EdgeType edgesType){
            this->kernel = std::move(kernel);
            this->edgeType = edgesType;
        }
        FImage apply(const FImage &image);
    private:
        Kernel kernel;
        EdgeType edgeType;
};

#endif // CONVULSION_H
