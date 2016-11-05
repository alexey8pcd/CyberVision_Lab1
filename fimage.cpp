#include "fimage.h"
#include "imageutil.h"

FImage::FImage(){
    buffer = unique_ptr<float[]>(new float[0]);
    width = 0;
    height = 0;
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
    int length = getLength();
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

FImage::FImage(FImage &&image) {
    this->width = image.width;
    this->height = image.height;
    this->buffer = std::move(image.buffer);
}

FImage::FImage(int width, int height){
    this->width = width;
    this->height = height;    
    this->buffer = unique_ptr<float[]>(new float[width * height]);
}

FImage::FImage(const FImage &image){
    this->width = image.width;
    this->height = image.height;
    this->buffer = unique_ptr<float[]>(new float[width * height]);
    int length = width * height;
    for (int i = 0; i < length; ++i) {
        buffer[i] = image.buffer[i];
    }
}

FImage::FImage(const QImage &image){
    this->width = image.width();
    this->height = image.height();
    this->buffer = unique_ptr<float[]>(new float[width * height]);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            buffer[x + width * y] = ImageUtil::toFloatValue(image.pixel(x, y));
        }
    }
}

float * FImage::getBuffer() {
    return buffer.get();
}

int FImage::getHeight() const {
    return height;
}

int FImage::getWidth() const {
    return width;
}

int FImage::getLength() const{
    return width * height;
}

float FImage::getValue(int x, int y) const{
    return buffer[x + width * y];
}

void FImage::setValue(int x, int y, float value) {
    buffer[x + width * y] = value;
}

void FImage::textOut()
{
    cout << "Image: " << this << "[" << endl;

    for (int i = 0; i < width; ++i) {
        for(int j = 0; j < height; ++j){
            int value =(int)(100 * buffer[i + width * j]);
            cout << ((float)value)/100 << " ";
        }
        cout << endl;
    }
    cout << "]" << endl;
}
