#include "convolution.h"



Convolution::Convolution(){

}

float Convolution::applyToPixel(FImage &image, int x, int y,
                                Kernel &kernel, EdgeType edgesType) {
    int width = image.getWidth();
    int height = image.getHeight();
    int kernelWidth = kernel.getWidth();
    int kernelHalfWidth = kernelWidth / 2;
    int kernelHeight = kernel.getHeight();
    int kernelHalfHeight = kernelHeight / 2;

    float result = 0;
    for (int i = 0, dx = kernelHalfWidth; i < kernelWidth; ++i, --dx){
        for(int j = 0, dy = kernelHalfHeight; j < kernelHeight; ++j, --dy){
            int xi = x + dx;
            int yj = y + dy;
            float intensity = 0;
            switch(edgesType){
                case EdgeType_COPY_EDGES:
                    xi = xi < 0 ? 0 : xi >= width ? width - 1 : xi;
                    yj = yj < 0 ? 0 : yj >= height ? height - 1 : yj;
                    intensity = image.getValue(xi, yj);
                    break;
                case EdgeType_WRAP_EDGES:
                    xi = xi < 0 ? xi += width : xi >= width ? xi -=width : xi;
                    yj = yj < 0 ? yj += height : yj >= height ? yj -=height : yj;
                    intensity = image.getValue(xi, yj);
                    break;
                case EdgeType_MIRROR_EDGES:
                    xi = xi < 0 ? -xi : xi >= width ? 2 * width - xi : xi;
                    yj = yj < 0 ? -yj : yj >= height ? 2 * height - yj : yj;
                    intensity = image.getValue(xi, yj);
                    break;
                default:
                    if(xi < 0 || yj < 0 || xi >= width ||yj >= height) {
                        intensity = 0;
                    }else{
                        intensity = image.getValue(xi, yj);
                    }
            }
            result += intensity * kernel.getValue(i, j);
        }
    }
    return result;
}

FImage Convolution::apply(FImage &image, Kernel &kernel,
                            EdgeType edgesType){
    int width = image.getWidth();
    int height = image.getHeight();

    FImage resultImage(width, height);
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            float result = applyToPixel(image, x, y, kernel, edgesType);
            resultImage.setValue(x, y, result);
        }
    }
    return resultImage;
}

//QImage Convolution::applyGauss(const QImage &image, const double sigma){
//    const int width = image.width();
//    const int height = image.height();
//    const int length = width * height;
//    const double rr2 = sigma * sigma * 2;
//    const double alpha = 1 / (rr2 * 3.141592);
//    int radius = (int)(sigma * 3);
//    double* output = new double[length];
//    double min = 1 << 30;
//    double max = 0;
//    for(int y = 0; y < height; ++y) {
//        int index = y * width;
//        for(int x = 0; x < width; ++x) {
//            double resultX = 0;
//            double resultY = 0;
//            for (int i = -radius, di = radius; i < radius; ++i, --di){
//                int xi = x + di;
//                int yi = y + di;
//                double coeff = alpha * exp(-i * i / rr2);
//                if (xi >= 0 && xi < width) {
//                    QRgb bright = image.pixel(xi, y);
//                    QRgb intensivity = ImageUtil::toGray(bright);
//                    resultX += coeff * intensivity;
//                }

//                if (yi >=0 && yi < height) {
//                    QRgb bright = image.pixel(x, yi);
//                    QRgb intensivity = ImageUtil::toGray(bright);
//                    resultY += coeff * intensivity;
//                }
//            }
//            double intensivity = resultX * resultY;
//            if(intensivity > max){
//                max = intensivity;
//            }
//            if(intensivity < min){
//                min = intensivity;
//            }
//            output[index++] = intensivity;
//        }
//    }
//    QRgb* result = normalizeD(output, min, max, length);
//    QImage resultImage = ImageUtil::toImage(result, width, height);
//    delete output;
//    delete result;
//    return resultImage;
//}


