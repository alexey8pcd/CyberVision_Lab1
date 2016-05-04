#include "QtGui"
class ImageUtil{
    public:
        static QPixmap toGrayscale(QPixmap pixmap){
            QImage image = pixmap.toImage();
            const QImage grayscale = image.convertToFormat(QImage::Format_Grayscale8);
            return QPixmap::fromImage(grayscale);
        }

};

