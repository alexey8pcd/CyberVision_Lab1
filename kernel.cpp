#include "kernel.h"

void Kernel::setWidth(int value) {
    this->width = value;
}

void Kernel::setHeight(int value) {
    this->heigth = value;
}

int Kernel::getWidth() const {
    return width;
}

int Kernel::getHeight() const {
    return heigth;
}

float *Kernel::getValues() {
    return values.get();
}

float Kernel::getValue(int i, int j) const {
    return values[i + width * j];
}

Kernel Kernel::createGaussKernel(double sigma) {
    const double s2 = sigma * sigma * 2;
    const double alpha = 1 / (s2 * 3.141592);
    int radius = (int)(sigma * 3);
    int size = 2 * radius + 1;
    Kernel kernel(size);
    for(int i = -radius, x = 0; i <= radius; ++i, ++x) {
        for(int j = -radius, y = 0; j <= radius; ++j, ++y) {
            kernel.values[x + size * y]
                    = (alpha * exp(-((float)i * i + j * j) / s2));
        }
    }
    return kernel;
}

Kernel Kernel::createGaussSeparateKernelX(double sigma) {
    const double s2 = sigma * sigma * 2;
    const double alpha = 1 / (sigma * sqrt(3.141592 * 2));
    int halfWidth = (int)(sigma * 3);
    int width = 2 * halfWidth + 1;
    Kernel kernel(width, 1);
    if(width == 1){
        kernel.values[0] = 1;
    }else{
        for (int i = -halfWidth, x = 0; i <= halfWidth; ++i, ++x) {
            kernel.values[x] = (alpha * exp(-(float)i * i / s2));
        }
    }

    return kernel;
}

Kernel Kernel::createGaussSeparateKernelY(double sigma) {
    Kernel&& xKernel = createGaussSeparateKernelX(sigma);
    xKernel.setHeight(xKernel.getWidth());
    xKernel.setWidth(1);
    return xKernel;
}

Kernel Kernel::createSobelKernelX() {
    float sobelXValues[] = {-1, 0, 1,
                            -2, 0, 2,
                            -1, 0, 1};
    Kernel kernel(3);
    for (int i = 0; i < 9; ++i) {
        kernel.values[i] = sobelXValues[i];
    }    
    return kernel;
}

Kernel Kernel::createSobelKernelY() {
    float sobelYValues[] = {-1, -2, -1,
                            0, 0, 0,
                            1, 2, 1};
    Kernel kernel(3);
    for (int i = 0; i < 9; ++i) {
        kernel.values[i] = sobelYValues[i];
    }
    return kernel;
}

Kernel::Kernel(int radius) {
    if(radius < 0){
        radius = 0;
    }
    this->values = unique_ptr<float[]>(new float[radius * radius]);
    this->width = radius;
    this->heigth = radius;
}

Kernel::Kernel(Kernel &&kernel){
    this->width = kernel.width;
    this->heigth = kernel.heigth;
    this->values = move(kernel.values);
}

Kernel::Kernel(const Kernel& kernel){
    this->width = kernel.width;
    this->heigth = kernel.heigth;
    this->values = unique_ptr<float[]>(new float[width * heigth]);
    int size = width * heigth;
    for (int i = 0; i < size; ++i) {
        values[i] = kernel.values[i];
    }
}

Kernel::Kernel(int width, int height) {
    if(width < 0){
        width = 0;
    }
    if(height < 0){
        height = 0;
    }
    this->values = unique_ptr<float[]>(new float[width * height]);
    this->width = width;
    this->heigth = height;
}

Kernel::Kernel(){
    this->width = 0;
    this->heigth = 0;
    this->values = unique_ptr<float[]>(new float[0]);
}

Kernel::~Kernel() {

}

