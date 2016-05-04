#ifndef CONVULSION_H
#define CONVULSION_H

#include "qimage.h"
#include "kernel.h"
class Convulsion
{
    public:
        Convulsion();
        static QImage apply(QImage &image, Kernel &kernel);
        static QImage normalize(int* buffer, int min, int max,
                                int width, int heigth);
};

#endif // CONVULSION_H
