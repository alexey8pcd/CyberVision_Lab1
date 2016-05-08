#ifndef FIMAGE_H
#define FIMAGE_H
#include "qimage.h"

class FImage
{
private:
    float * buffer;
    int width;
    int height;
    int length;
public:
    FImage(QImage &image);
    FImage(int width, int height);
    FImage(float * buffer, int width, int height);
//    FImage(FImage &toCopy);
    FImage();
    ~FImage();
    QImage toQImage();
    void normalize();
    float * getBuffer();
    int getWidth();
    int getHeight();
    int getLength();
    float getValue(int x, int y);
    void setValue(int x, int y, float value);
};

#endif // FIMAGE_H
