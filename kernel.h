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
    Kernel();
    Kernel(float * values, int width, int height);
    Kernel(float * values, int radius);
    ~Kernel();

    static Kernel * createGaussKernel(double sigma);
    static Kernel * createGaussSeparateKernelX(double sigma);
    static Kernel * createGaussSeparateKernelY(double sigma);
};

#endif // KERNEL_H
