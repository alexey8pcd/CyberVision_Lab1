#include "QtGui"
#include "qimage.h"
class ImageUtil {
    public:
        static const int MAX_INTENSITY = 255;
        static QRgb toQRgb(float value);
        static QImage toImage(QRgb *buffer, int width, int height);
        static QPixmap toGrayscale(QPixmap pixmap);
        static QImage downscale(QImage &image, int level);
        static float toFloatValue(QRgb bright);
};

