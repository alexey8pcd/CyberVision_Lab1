#ifndef KERNEL_H
#define KERNEL_H
#include "math.h"

class Kernel
{
private:
    int width;
    int heigth;
    float * values;
    void setWidth(int value);
    void setHeight(int value);
public:
    void setValues(float * values, int width, int height);
    void setValues(float * values, int radius);
    int getWidth();
    int getHeight();
    float *getValues();
    float getValue(int i, int j);
    static Kernel * createGaussKernel(double sigma);
    static Kernel * createGaussSeparateKernelX(double sigma);
    static Kernel * createGaussSeparateKernelY(double sigma);
    Kernel();
    Kernel(float * values, int width, int height);
    Kernel(float * values, int radius);
};

#endif // KERNEL_H
