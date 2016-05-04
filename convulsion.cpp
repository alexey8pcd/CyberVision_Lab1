#include "convulsion.h"

Convulsion::Convulsion(){

}

QImage Convulsion::apply(QImage &image, Kernel &kernel){
    int width = image.width();
    int height = image.height();
    int* buffer = new int[width*height];
    int min = 0;
    int max = 0;
    int coreSize = kernel.getCoreSize();
    int halfSize = coreSize / 2;
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            int result = 0;
            for (int i = 0, dx = halfSize; i < coreSize; ++i, --dx){
                for(int j = 0, dy = halfSize; j < coreSize; ++j, --dy){
                    int xi = x + dx;
                    int yj = y + dy;
                    result += buffer[xi+width*yj] * kernel.getIntensity(i,j);
                }
            }
            if(result < min){
                min = result;
            }
            if(result > max){
                max = result;
            }
        }
    }
    return normalize(buffer, min, max, width, height);
}

QImage Convulsion::normalize(int *buffer, int min, int max,
                             int width, int height){
    QImage image = QImage(width, height, QImage::Format_Grayscale8);
    if(min != 0){
        for(int i = 0; i < width; ++i){
            for(int j = 0; j < height; ++j){
                buffer[i][j] -= min;
            }
        }
    }
    if(max != 255){
        for(int x = 0; x < width; ++x){
            for(int y = 0; y < height; ++y){
                double value = buffer[x][y];
                value/=255;
                image.setPixel(x, y, (unsigned)(value*255));
            }
        }
    }
    delete buffer;
    return image;
}

