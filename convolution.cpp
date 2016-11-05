#include "convolution.h"


FImage Convolution::apply(const FImage &image){
    int width = image.getWidth();
    int height = image.getHeight();

    FImage resultImage(width, height);
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            int width = image.getWidth();
            int height = image.getHeight();
            int kernelWidth = kernel.getWidth();
            int kernelHalfWidth = kernelWidth / 2;
            int kernelHeight = kernel.getHeight();
            int kernelHalfHeight = kernelHeight / 2;

            float result = 0;
            for (int i = 0, dx = kernelHalfWidth; i < kernelWidth; ++i, --dx){
                for(int j = 0, dy = kernelHalfHeight; j < kernelHeight; ++j, --dy){
                    int xi = ImageUtil::handleEdgeEffect(x + dx, width, edgeType);
                    int yj = ImageUtil::handleEdgeEffect(y + dy, height, edgeType);
                    if(ImageUtil::insideImage(xi, yj)) {
                        float intensity = image.getValue(xi, yj);
                        result += intensity * kernel.getValue(i, j);
                    }
                }
            }
            resultImage.setValue(x, y, result);
        }
    }
    return resultImage;
}

