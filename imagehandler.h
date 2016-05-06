#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include "kernel.h"
#include "QImage"
#include "QPixmap"
#include "QDebug"
#include "QRgb"
#include "qmath.h"
#include "convolution.h"
class ImageHandler
{
    private:
        QImage original;
        int width;
        int height;

    public:
        QPixmap getOriginal();
        QPixmap getFilteredSobel();
        QPixmap getFilteredSobelX();
        QPixmap getFilteredSobelY();
        QPixmap getFilteredGauss(double sigma);
        void setImage(QString fileName);
        QImage getImage();
        ImageHandler();
        ImageHandler(QString fileName);
};

#endif // IMAGEHANDLER_H
