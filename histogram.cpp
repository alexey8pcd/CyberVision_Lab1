#include "histogram.h"

void Histogram::addValueToBasket(int index0, int index1, float value) {
    if(index0 < 0){
        index0 += basketsCount;
    }
    if(index1 < 0){
        index1 += basketsCount;
    }
    if(index0 >= basketsCount){
        index0 -= basketsCount;
    }
    if(index1 >= basketsCount){
        index1 -= basketsCount;
    }
    baskets[index0] += value / 2;
    baskets[index1] += value / 2;
}

void Histogram::addValue(float phi, float value) {
    if(phi != phi){//Nan check
        return;
    }
    while(phi < 0){
        phi += PI_2;
    }
    while(phi > PI_2){
        phi -= PI_2;
    }
    float currentPhi = 0;
    float nextPhi = dphi;
    for (int i = 0; i < basketsCount; ++i) {
        if(phi > currentPhi && phi <= nextPhi){
            float dp = phi - currentPhi;
            float dn = nextPhi - phi;
            if(dp > dn){
                addValueToBasket(i, i - 1, value);
            } else {
                addValueToBasket(i, i + 1, value);
            }
            return;
        }else{
            currentPhi = nextPhi;
            nextPhi += dphi;
        }
    }
}

void Histogram::normalize() {
    float module = getModule();
    for (int i = 0; i < basketsCount; ++i) {
        baskets[i] /= module;
    }
}

void Histogram::normalize(float denominator) {
    for (int i = 0; i < basketsCount; ++i) {
        baskets[i] /= denominator;
    }
}

float Histogram::getDistance(Histogram &histogram) {
    if (basketsCount == histogram.basketsCount){
        float sum = 0;
        for (int i = 0; i < basketsCount; ++i) {
            float d = baskets[i] - histogram.baskets[i];
            sum += d * d;
        }
        return sqrtf(sum);
    }else{
        return -1;
    }
}

float Histogram::getModule() {
    float sum = 0;
    for (int i = 0; i < basketsCount; ++i) {
        sum += baskets[i] * baskets[i];
    }
    return sqrtf(sum);
}

Histogram::Histogram(int basketsCount) {
    this->basketsCount = basketsCount;
    this->baskets = new float[basketsCount];
    for (int i = 0; i < basketsCount; ++i) {
        baskets[i] = 0;
    }
    this->dphi = PI_2 / basketsCount;
}

Histogram::Histogram() {
    this->basketsCount = 0;
    this->dphi = 0;
    this->baskets = new float[0];
}

