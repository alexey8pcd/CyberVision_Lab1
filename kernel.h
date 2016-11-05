#ifndef KERNEL_H
#define KERNEL_H
#include "math.h"
#include "QtCore"
#include "memory"
using namespace std;
class Kernel
{
private:
    int width;
    int heigth;
    unique_ptr<float[]> values;
    void setWidth(int value);
    void setHeight(int value);
public:
    int getWidth() const;
    int getHeight() const;
    float *getValues();
    float getValue(int i, int j) const;
    Kernel();
    Kernel(int radius);
    Kernel(int width, int height);
    Kernel(const Kernel &kernel);
    Kernel(Kernel &&kernel);
    Kernel& operator=(Kernel &&kernel){
        this->width = kernel.width;
        this->heigth = kernel.heigth;
        values = move(kernel.values);
        return *this;
    }

    ~Kernel();

    static Kernel createGaussKernel(double sigma);
    static Kernel createGaussSeparateKernelX(double sigma);
    static Kernel createGaussSeparateKernelY(double sigma);
    static Kernel createSobelKernelX();
    static Kernel createSobelKernelY();
};

#endif // KERNEL_H
