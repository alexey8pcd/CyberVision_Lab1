#include "imageutil.h"

QRgb ImageUtil::toGray(QRgb bright){
    int red = (bright >> 16) & 0xFF;
    int green = (bright >> 8) & 0xFF;
    int blue = bright & 0xFF;
    return ((QRgb)(red * 0.299 + green
                       * 0.587 + blue * 0.114)) & 0xFF;
}

QPixmap ImageUtil::toGrayscale(QPixmap pixmap){
    QImage image = pixmap.toImage();
    const QImage grayscale = image.convertToFormat(QImage::Format_Grayscale8);
    return QPixmap::fromImage(grayscale);
}

QImage ImageUtil::toImage(QRgb *buffer, int width, int height){
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

QImage ImageUtil::downscale(QImage &image, int level){
    int value = 1 << level;
    int width2 = image.width() / value;
    int height2 = image.height() / value;
    QImage result(width2, height2, image.format());
    for(int x = 0; x < width2; ++x) {
        for(int y = 0; y < height2; ++y) {
            result.setPixel(x, y, image.pixel(x * value, y * value));
        }
    }
    return result;
}
