#include "kernel.h"

void Kernel::setWidth(int value) {
    this->width = value;
}

void Kernel::setHeight(int value) {
    this->height = value;
}

int Kernel::getWidth() const {
    return width;
}

int Kernel::getHeight() const {
    return height;
}

float *Kernel::getValues() {
    return values.get();
}

float Kernel::getValueByIndexes(int i, int j) const {
    int index = i + width * j;
//    if(index < 0 || index >= width * height){
//        qDebug() << i << ";" << j;
//    }
    return values[index];
}

float Kernel::getValue(int x, int y) const {
    int xi = x + width / 2;
    int yj = y + height / 2;
    int index = xi + width * yj;
//    if(index < 0 || index >= width * height){
//        qDebug() << "i: " << xi << ", j: " << yj;
//        qDebug() << "x: " << x << ", y: " << y;
//    }
    return values[index];
}

Kernel Kernel::createGaussKernel(double sigma) {
    const double s2 = sigma * sigma * 2;
    const double alpha = 1 / (s2 * 3.141592);
    int radius = (int)(sigma * 3 + 0.5);
    int size = 2 * radius + 1;
    Kernel kernel = Kernel(size);
    for(int i = -radius, x = 0; i <= radius; ++i, ++x) {
        for(int j = -radius, y = 0; j <= radius; ++j, ++y) {
            float value = alpha * exp(-((float)i * i + j * j) / s2);
            kernel.values[x + size * y] = value;
        }
    }
    return kernel;
}

Kernel Kernel::createGaussKernelByRadius(int radius){
    int size = 2 * radius + 1;
    double sigma = (double)radius / 3;
    const double s2 = sigma * sigma * 2;
    const double alpha = 1 / (s2 * 3.141592);
    Kernel kernel = Kernel(size);
    for(int i = -radius, x = 0; i <= radius; ++i, ++x) {
        for(int j = -radius, y = 0; j <= radius; ++j, ++y) {
            float value = alpha * exp(-((float)i * i + j * j) / s2);
            kernel.values[x + size * y] = value;
        }
    }
    return kernel;
}

Kernel Kernel::createGaussSeparateKernelX(double sigma) {
    const double s2 = sigma * sigma * 2;
    const double alpha = 1 / (sigma * sqrt(3.141592 * 2));
    int halfWidth = (int)(sigma * 3);
    int width = 2 * halfWidth + 1;
    Kernel kernel = Kernel(width, 1);
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
    Kernel xKernel = createGaussSeparateKernelX(sigma);
    xKernel.setHeight(xKernel.getWidth());
    xKernel.setWidth(1);
    return xKernel;
}

Kernel Kernel::createSobelKernelX() {
    float sobelXValues[] = {-1, 0, 1,
                            -2, 0, 2,
                            -1, 0, 1};
    Kernel kernel = Kernel(3);
    for (int i = 0; i < 9; ++i) {
        kernel.values[i] = sobelXValues[i];
    }    
    return kernel;
}

Kernel Kernel::createSobelKernelY() {
    float sobelYValues[] = {-1, -2, -1,
                            0, 0, 0,
                            1, 2, 1};
    Kernel kernel = Kernel(3);
    for (int i = 0; i < 9; ++i) {
        kernel.values[i] = sobelYValues[i];
    }
    return kernel;
}

Kernel Kernel::createSimpleGradientKernelX()
{
    Kernel kernel = Kernel(3, 1);
    kernel.getValues()[0] = -1;
    kernel.getValues()[1] = 0;
    kernel.getValues()[2] = 1;
    return kernel;
}

Kernel Kernel::createSimpleGradientKernelY()
{
    Kernel kernel = Kernel(1, 3);
    kernel.getValues()[0] = -1;
    kernel.getValues()[1] = 0;
    kernel.getValues()[2] = 1;
    return kernel;
}

Kernel::Kernel(int size) {
    if(size < 0){
        size = 0;
    }
    this->values = unique_ptr<float[]>(new float[size * size]);
    this->width = size;
    this->height = size;
}

Kernel::Kernel(Kernel &&kernel){
    this->width = kernel.width;
    this->height = kernel.height;
    this->values = move(kernel.values);
}

void Kernel::printDebug(){
    qDebug() << "Width: " << width << ", Height: " << height;
}

Kernel::Kernel(const Kernel& kernel){
    this->width = kernel.width;
    this->height = kernel.height;
    this->values = unique_ptr<float[]>(new float[width * height]);
    int size = width * height;
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
    this->height = height;
}

Kernel::Kernel(){
    this->width = 0;
    this->height = 0;
    this->values = unique_ptr<float[]>(new float[0]);
}

Kernel::~Kernel() {

}



