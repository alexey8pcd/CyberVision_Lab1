#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include "kernel.h"
#include "QImage"
#include "QPixmap"
#include "QDebug"
#include "QRgb"
#include "qmath.h"
#include "convulsion.h"
#include "sobelx.h"
class ImageHandler
{
    private:
        QImage original;
        int width;
        int height;
        const int xKernel[3][3] = {
            {-1,0,1},
            {-2,0,2},
            {-1,0,1}
        };
        const int yKernel[3][3] = {
            {-1,-2,-1},
            {0,0,0},
            {1,2,1}
        };
        QImage getImageFilteredSobelX();
        QImage getImageFilteredSobelY();
    public:
        QPixmap getOriginal();
        QPixmap getFilteredSobelX();
        QPixmap getFilteredSobelY();
        QPixmap getFilteredSobel();
        QPixmap getFilteredGauss(int gamma);
        ImageHandler();
        ImageHandler(QString fileName);
};

#endif // IMAGEHANDLER_H
