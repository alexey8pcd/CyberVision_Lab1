#include "imagehandler.h"

ImageHandler::ImageHandler(){

}

ImageHandler::ImageHandler(QString fileName){
    this->original = QImage(fileName).convertToFormat(QImage::Format_Grayscale8);
    this->width = original.width();
    this->height = original.height();
}

QPixmap ImageHandler::getOriginal(){
    return QPixmap::fromImage(original);
}

QImage ImageHandler::getImageFilteredSobelX(){
    QImage copy = QImage(width, height, QImage::Format_Grayscale8);
    Kernel* kernel =  new SobelX();
    return Convulsion::apply(copy, (*kernel) );
}

QImage ImageHandler::getImageFilteredSobelY(){
    QImage copy = QImage(width, height, QImage::Format_Grayscale8);
    for (int x = 1; x < width-1; ++x){
        for(int y = 1; y < height-1; ++y){
            int result = 0;
            for(int i = -1; i <= 1; ++i){
                for(int j = -1; j <= 1; ++j){
                    int xi = x-i;
                    int yj = y-j;
                    uint pixel= original.pixel(xi,yj);
                    result += pixel * yKernel[j + 1][j + 1];
                }
            }
            copy.setPixel(x,y, result);
        }
    }
    return copy;
}

QPixmap ImageHandler::getFilteredSobelX(){
    return QPixmap::fromImage(getImageFilteredSobelX());
}

QPixmap ImageHandler::getFilteredGauss(int gamma){
    return getOriginal();
}

QPixmap ImageHandler::getFilteredSobel(){
    QImage copy = original.copy(0, 0, width, height);
    QImage sx = getImageFilteredSobelX();
    QImage sy = getImageFilteredSobelY();
    for (int x = 1; x < width-1; ++x){
        for(int y = 1; y < height-1; ++y){
            uint resultX = sx.pixel(x,y) & 0xFF;
            uint resultY = sy.pixel(x,y) & 0xFF;
            int result = sqrt(resultX*resultX+resultY*resultY);
            int p = result << 16 | result << 8 | result;
            copy.setPixel(x,y, p);
        }
    }
    return QPixmap::fromImage(copy);
}

QPixmap ImageHandler::getFilteredSobelY(){
    return QPixmap::fromImage(getImageFilteredSobelY());
}


