#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include "kernel.h"
#include "QImage"
#include "QPixmap"
#include "QDebug"
#include "QRgb"
#include "qmath.h"
#include "convulsion.h"
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
        QPixmap getFilteredGauss(double gamma);
        ImageHandler();
        ImageHandler(QString fileName);
};

#endif // IMAGEHANDLER_H
