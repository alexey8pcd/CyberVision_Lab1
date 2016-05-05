#include "QtGui"
#include "qimage.h"
class ImageUtil{
    public:
        static QRgb toGray(QRgb bright){
            int red = (bright >> 16) & 0xFF;
            int green = (bright >> 8) & 0xFF;
            int blue = bright & 0xFF;
            return ((QRgb)(red * 0.299 + green
                               * 0.587 + blue * 0.114)) & 0xFF;
        }

        static QImage toImage(QRgb *buffer, int width, int height){
            QImage resultImage(width, height, QImage::Format_RGB32);
            for (int x = 1; x < width - 1; ++x){
                for(int y = 1; y < height - 1; ++y){
                    QRgb color = buffer[x + y * width];
                    color = (color << 16) | (color << 8) | color;
                    resultImage.setPixel(x, y, color);
                }
            }
            return resultImage;
        }

        static QPixmap toGrayscale(QPixmap pixmap){
            QImage image = pixmap.toImage();
            const QImage grayscale = image.convertToFormat(QImage::Format_Grayscale8);
            return QPixmap::fromImage(grayscale);
        }

};

