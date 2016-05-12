#include "descriptor.h"

Descriptor::Descriptor(int x, int y, int histogramCount) {
    this->x = x;
    this->y = y;
    this->histogramCount = histogramCount;
    this->histograms = new Histogram[histogramCount];
}

Descriptor::Descriptor(){
    this->histogramCount = 0;
    this->histograms = new Histogram[0];
}

void Descriptor::setHistogram(int index, Histogram &histogram) {
    if(index < 0 || index >= histogramCount){
        return;
    }
    histograms[index] = histogram;
}

float Descriptor::getModule() {
    float sum = 0;
    for (int i = 0; i < histogramCount; ++i) {
        sum += histograms[i].getModule();
    }
    return sqrtf(sum);
}

float Descriptor::getDistance(Descriptor& descriptor) {
    if (histogramCount == descriptor.histogramCount){
        float sum = 0;
        for (int i = 0; i < histogramCount; ++i) {
            float d = histograms[i].getDistance(descriptor.histograms[i]);
            sum += d * d;
        }
        return sqrtf(sum);
    }else{
        return -1;
    }
}



void Descriptor::normalize() {
    float eps = 1e-4;
    float module = getModule() + eps;
    for (int i = 0; i < histogramCount; ++i) {
        histograms[i].normalize(module);
    }
}

int Descriptor::getX() {
    return x;
}

int Descriptor::getY() {
    return y;
}

