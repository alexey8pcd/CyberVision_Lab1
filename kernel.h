#ifndef KERNEL_H
#define KERNEL_H
#include "math.h"
#include "QtCore"
#include "memory"
#include <iostream>
using namespace std;
class Kernel
{
private:
    int width;
    int height;
    unique_ptr<float[]> values;

    void setHeight(int value);
public:
    void setWidth(int value);
    int getWidth() const;
    int getHeight() const;
    float *getValues();
    float getValueByIndexes(int rowIndex, int columnIndex) const;
    float getValue(int x, int y) const;
    Kernel();
    Kernel(int size);
    Kernel(int width, int height);
    Kernel(const Kernel &kernel);
    Kernel(Kernel &&kernel);
    Kernel& operator=(Kernel &&kernel){
        this->width = kernel.width;
        this->height = kernel.height;
        values = move(kernel.values);
        return *this;
    }
    void printDebug();
    ~Kernel();

    static Kernel createGaussKernelByRadius(int radius);
    static Kernel createGaussKernel(double sigma);
    static Kernel createGaussSeparateKernelX(double sigma);
    static Kernel createGaussSeparateKernelY(double sigma);
    static Kernel createSobelKernelX();
    static Kernel createSobelKernelY();
    static Kernel createSimpleGradientKernelX();
    static Kernel createSimpleGradientKernelY();
};

#endif // KERNEL_H
