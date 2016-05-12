#include "pyramid.h"

PyramidBuilder::PyramidBuilder()
{

}

PyramidBuilder::PyramidBuilder(int octaveCount,
                               int levelPerOctave, double sigmaAlpha){
    this->octaveCount = octaveCount;
    this->levelPerOctave = levelPerOctave;
    this->sigmaAlpha = sigmaAlpha;
}

void PyramidBuilder::createOctaves(FImage &image, EdgeType type){
    double k = pow(2,(1. / levelPerOctave));
    int l = 0;
    int scale = 1;
    for(int i = 0; i < octaveCount; ++i) {
        for(int j = 0; j < levelPerOctave; ++j) {
            double sigmaNext = sigmaAlpha * pow(k, l++);
            qInfo() << "Octave: " << (i + 1) << ", Level: "
                    << (j + 1) << ", Sigma: " << sigmaNext;
            Kernel kernelX = Kernel::createGaussSeparateKernelX(sigmaNext);
            FImage imgX = Convolution::apply(image, kernelX, type);
            Kernel kernelY = Kernel::createGaussSeparateKernelY(sigmaNext);
            FImage imgY = Convolution::apply(imgX, kernelY, type);
            imgY.normalize();
            if(scale > 1){
                FImage img = imgY.downscale(scale);
                QImage result = img.toQImage();
                result.save(QString::number(i * 10 + j).append(".png"),"png");
            } else {
                QImage result = imgY.toQImage();
                result.save(QString::number(i * 10 + j).append(".png"),"png");
            }
        }
        scale *= 2;
    }
}
