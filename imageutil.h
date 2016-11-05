#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#include "QtGui"
#include "qimage.h"
#include "datatypes.h"
class ImageUtil {
    public:        
        static const int MAX_INTENSITY = 255;
        static const int OUTSIDE_PIXEL_ADDRESS = 0x7FFFFFFF;
        static bool insideImage(int xIndex, int yIndex);
        static QRgb toQRgb(float value);
        static QImage toImage(QRgb *buffer, int width, int height);
        static QPixmap toGrayscale(QPixmap pixmap);
        static float toFloatValue(QRgb bright);
        static int handleEdgeEffect(int value, int size, EdgeType type);
};

#endif
