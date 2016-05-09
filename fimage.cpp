#include "fimage.h"
#include "imageutil.h"

FImage::FImage(){
    buffer = new float[0];
    width = 0;
    height = 0;
    length = 0;
}

FImage::~FImage(){
}

QImage FImage::toQImage(){
    QImage image(width, height, QImage::Format_RGB32);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            image.setPixel(x, y, ImageUtil::toQRgb(getValue(x, y)));
        }
    }
    return image;
}

void FImage::normalize() {
    float min = buffer[0];
    float max = buffer[0];
    for (int i = 0; i < length; ++i) {
        min = buffer[i] < min ? buffer[i] : min;
        max = buffer[i] > max ? buffer[i] : max;
    }

    float diff = max -  min;
    for (int i = 0; i < length; ++i) {
        float value = buffer[i] - min;
        buffer[i] = value / diff;
    }
}

FImage FImage::downscale(int level)
{
    int value = 1 << level;
    int width2 = width / value;
    int height2 = height / value;
    FImage image(width2, height2);
    for(int x = 0; x < width2; ++x) {
        for(int y = 0; y < height2; ++y) {
            image.setValue(x, y, getValue(x * value, y * value));
        }
    }
    return image;
}

FImage::FImage(float *buffer, int width, int height){
    this->buffer = buffer;
    this->width = width;
    this->height = height;
    this->length = width * height;
}

FImage::FImage(int width, int height){
    this->width = width;
    this->height = height;
    this->length = width * height;
    this->buffer = new float[length];
}

FImage::FImage(QImage &image){
    this->width = image.width();
    this->height = image.height();
    this->length = width * height;
    this->buffer = new float[length];
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            buffer[x + width * y] = ImageUtil::toFloatValue(image.pixel(x, y));
        }
    }
}

float * FImage::getBuffer() {
    return buffer;
}

int FImage::getHeight() {
    return height;
}

int FImage::getWidth() {
    return width;
}

int FImage::getLength() {
    return length;
}

float FImage::getValue(int x, int y) {
    return buffer[x + width * y];
}

void FImage::setValue(int x, int y, float value) {
    buffer[x + width * y] = value;
}
