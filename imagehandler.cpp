#include "imagehandler.h"

ImageHandler::ImageHandler(){

}

ImageHandler::ImageHandler(QString fileName){
    this->original = QImage(fileName).convertToFormat(QImage::Format_ARGB32);
    this->width = original.width();
    this->height = original.height();
}

void ImageHandler::setImage(QString fileName){
    this->original = QImage(fileName).convertToFormat(QImage::Format_ARGB32);
    this->width = original.width();
    this->height = original.height();
}

QPixmap ImageHandler::getOriginal(){
    return QPixmap::fromImage(original);
}

QPixmap ImageHandler::getFilteredGauss(double sigma){
    return QPixmap::fromImage(Convolution::applyGauss(original, sigma));
}

QPixmap ImageHandler::getFilteredSobel(){
    QImage copy = Convolution::applySobel(original);
    return QPixmap::fromImage(copy);
}

QPixmap ImageHandler::getFilteredSobelX(){
    const int sobelKernelX[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    QImage copy = Convolution::applySobelKernel(original, sobelKernelX);
    return QPixmap::fromImage(copy);
}

QPixmap ImageHandler::getFilteredSobelY(){
    const int sobelKernelY[3][3] = {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };
    QImage copy = Convolution::applySobelKernel(original, sobelKernelY);
    return QPixmap::fromImage(copy);
}


