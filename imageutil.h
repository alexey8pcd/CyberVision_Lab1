#include "QtGui"
#include "qimage.h"
class ImageUtil{
    public:
        static QRgb toGray(QRgb bright);
        static QImage toImage(QRgb *buffer, int width, int height);
        static QPixmap toGrayscale(QPixmap pixmap);
        static QImage downscale(QImage &image, int level);
};

