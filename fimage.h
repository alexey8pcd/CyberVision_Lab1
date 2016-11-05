#ifndef FIMAGE_H
#define FIMAGE_H
#include "qimage.h"
#include "memory"
#include <iostream>
using namespace std;

class FImage
{
private:
    unique_ptr<float[]> buffer;
    int width;
    int height;
public:
    FImage();
    FImage(const QImage &image);
    FImage(int width, int height);
    FImage(const FImage &image);
    FImage(FImage &&image);
    FImage& operator=(FImage &&image){
        this->width = image.width;
        this->height = image.height;
        buffer = move(image.buffer);
        return *this;
    }
    QImage toQImage();
    void normalize();
    FImage downscale(int level);
    float * getBuffer();
    int getWidth() const;
    int getHeight() const;
    int getLength() const;
    float getValue(int x, int y) const;
    void setValue(int x, int y, float value);
    void textOut();
};

#endif // FIMAGE_H
