#include "kernel.h"

void Kernel::setCoefficients(int *coefficients){
    this->coefficients = coefficients;
}

void Kernel::toSobelX(){
    int coefficients[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    this->coefficients = coefficients;
    size = 3;
}

void Kernel::toSobelY(){
    int coefficients[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {-1, 2, 1}
    };
    this->coefficients = coefficients;
    size = 3;
}

void Kernel::setSize(int size){
    this->size = size;
}

Kernel::Kernel(){
    coefficients = 1;
    size = 1;
}
