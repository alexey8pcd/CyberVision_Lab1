#include "common.h"

int calculateNumberOfNeighbornBasket(float halfBasketSize, float mod,
                                     int basketNumber, int basketCount) {
    int basketNumberNei;
    if(mod >= halfBasketSize) {
        basketNumberNei = basketNumber + 1;
    } else {
        basketNumberNei = basketNumber - 1;
    }
    if(basketNumberNei == basketCount){
        basketNumberNei = 0;
    }
    if(basketNumberNei < 0){
        basketNumberNei = basketCount - 1;
    }
    return basketNumberNei;
}

float maxInterp3f(float y1, float y2, float y3) {
    //x1 = 0, x2 = 1, x3 = 2, формула упрощена(c = y1)
    float a = (y1 + y3 - 2 * y2) / 2.f;
    float b = y2 - y1 - a;
    float x0 = -b / (2 * a);
    float yMax = a * x0 * x0 + b * x0 + y1;
    return yMax;
}
