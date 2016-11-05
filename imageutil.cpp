#include "imageutil.h"

bool ImageUtil::insideImage(int xIndex, int yIndex){
    return xIndex != OUTSIDE_PIXEL_ADDRESS
                     && yIndex != OUTSIDE_PIXEL_ADDRESS;
}

QRgb ImageUtil::toQRgb(float value){
    QRgb bright = (QRgb) (value * MAX_INTENSITY) & 0xFF;
    int red = bright << 16;
    int green = bright << 8;
    int blue = bright;
    return (QRgb)(red + green + blue);
}

float ImageUtil::toFloatValue(QRgb bright){
    int red = (bright >> 16) & 0xFF;
    int green = (bright >> 8) & 0xFF;
    int blue = bright & 0xFF;
    QRgb raw = ((QRgb)(0.299 * red + 0.587 * green
                       + 0.114 * blue)) & 0xFF;
    return ((float)raw) / MAX_INTENSITY;
}

QPixmap ImageUtil::toGrayscale(QPixmap pixmap){
    QImage&& image = pixmap.toImage();
    const QImage& grayscale = image.convertToFormat(QImage::Format_Grayscale8);
    return QPixmap::fromImage(grayscale);
}

QImage ImageUtil::toImage(QRgb *buffer, int size, int height){
    QImage resultImage(size, height, QImage::Format_RGB32);
    for (int x = 1; x < size - 1; ++x){
        for(int y = 1; y < height - 1; ++y){
            QRgb color = buffer[x + y * size];
            color = (color << 16) | (color << 8) | color;
            resultImage.setPixel(x, y, color);
        }
    }
    return resultImage;
}

int ImageUtil::handleEdgeEffect(int value, int size, EdgeType type) {
    switch(type){
        case EdgeType_COPY_EDGES:
            return value < 0 ? 0 : value >= size ? size - 1 : value;
        case EdgeType_WRAP_EDGES:
            return value < 0 ? value += size : value >= size
                                        ? value -=size : value;
        case EdgeType_MIRROR_EDGES:
            return value < 0 ? -value : value >= size
                               ? 2 * size - value - 1 : value;
        default:
            return value < 0 ? OUTSIDE_PIXEL_ADDRESS : value >= size
                               ? OUTSIDE_PIXEL_ADDRESS : value;
    }
}
