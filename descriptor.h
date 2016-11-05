#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include "memory"
#include "qdebug.h"
#include <iostream>
using namespace std;

template <int length> class Descriptor {

    private:
        float baskets[length] = {0.};
        int x;
        int y;
    public:
        Descriptor(int x, int y){
            this->x = x;
            this->y = y;
        }

        Descriptor(){
            this->x = 0;
            this->y = 0;
        }

        void addValue(int index, float value){
            if(index >= 0 && index < length){
                baskets[index] += value;
            }
        }

        void cutByThreshold(float value){
            for (int i = 0; i < length; ++i) {
                if(baskets[i] > value){
                    baskets[i] = value;
                }
            }
        }

        float getModule() const{
            float sum = 0;
            for (int i = 0; i < length; ++i) {
                sum += baskets[i] * baskets[i];
            }
            return sqrtf(sum);
        }

        void printInfo(){
            cout << "Descriptor: " << x << "; " <<
                        y << endl << "Elements: [";
            for (int i = 0; i < length; ++i) {
                cout << baskets[i] << " ";
            }
            cout << "]" << endl << endl;
         }

        float getDistance(Descriptor &descriptor) const{
            float sum = 0;
            for (int i = 0; i < length; ++i) {
                float df = baskets[i] - descriptor.baskets[i];
                sum += df * df;
            }
            return sum;
        }

        void normalize(){
            float eps = 1e-4;
            float module = getModule() + eps;
            for (int i = 0; i < length; ++i) {
                baskets[i] /= module;
            }
        }

        void normalizeHistogram(const int histogramSize, int indexOfHistorgam){
            if(indexOfHistorgam < 0 || indexOfHistorgam >= length / histogramSize) {
                qDebug() << "Индекс гистограммы вне границ дескриптора, "
                            + indexOfHistorgam;
                return;
            }
            if(histogramSize <= 0 || histogramSize > length){
                qDebug() << "Неверный размер гистограммы, " + histogramSize;
                return;
            }
            float histogramVectorLength = 0;
            int start = indexOfHistorgam * histogramSize;
            for (int i = start; i < start + histogramSize; ++i) {
                histogramVectorLength += baskets[i] * baskets[i];
            }
            histogramVectorLength = sqrtf(histogramVectorLength);
            if(histogramVectorLength != 0){
                for (int i = start; i < start + histogramSize; ++i) {
                    baskets[i] /= histogramVectorLength;
                }
            }

        }

        int getX() const{
             return x;
        }

        int getY() const{
             return y;
        }

};

#endif // DESCRIPTOR_H
