#include "pyramidbuilder.h"

PyramidBuilder::PyramidBuilder() {

}

float PyramidBuilder::getL(int x, int y, float sigma) {
    int layer = (int)round(log2(sigma / sigmaAlias) / (1. / levelsPerOctave));
    if(layer < 0){
        layer = 0;
    }
    if(layer >= levelsPerOctave * octavesCount){
        layer = levelsPerOctave * octavesCount - 1;
    }
    int currentOctave = layer / levelsPerOctave;
    x >>= currentOctave;
    y >>= currentOctave;
    FImage& img = imagesBuffer[0];
    int xwidth = img.getWidth() >> currentOctave;
    return imagesBuffer[layer].getBuffer()[x + y * xwidth];
}

PyramidBuilder::PyramidBuilder(FImage &image, EdgeType type,
                 int levelPerOctave, double sigmaAlias, double sigma){
    this->levelsPerOctave = levelPerOctave;
    this->sigmaAlias = sigmaAlias;
    this->sigma = sigma;
    this->type = type;
    this->image = std::move(image);
    this->octavesCount = round(log2(image.getWidth() > image.getHeight()
                              ? image.getWidth() : image.getHeight())) - 2;
    if(octavesCount <= 0) {
        imagesBuffer = make_unique<FImage[]>(0);
    } else {
        imagesBuffer = make_unique<FImage[]>(octavesCount * (levelsPerOctave + 3));
    }
}

void PyramidBuilder::save()
{
    for (int i = 0; i < octavesCount * levelsPerOctave; ++i) {
        FImage& img = imagesBuffer[i];
        QImage result = img.toQImage();
        result.save(QString::number(i).append(".png"),"png");
    }
}

FImage PyramidBuilder::prepareImage(double initSigma) {
    Kernel kernelX = Kernel::createGaussSeparateKernelX(initSigma);
    Kernel kernelY = Kernel::createGaussSeparateKernelY(initSigma);
    Convolution convX = Convolution(kernelX, type);
    Convolution convY = Convolution(kernelY, type);
    FImage imgX = convX.apply(image);
    FImage img = convY.apply(imgX);
    return img;
}

void PyramidBuilder::createOctavesWithAddLevels(){
    double sdiff = sigma * sigma - sigmaAlias * sigmaAlias;
    if(sdiff < 0){
        qDebug() << "Неверные параметры для построения пирамид";
        return;
    }

}

void PyramidBuilder::createOctaves() {
    double sdiff = sigma * sigma - sigmaAlias * sigmaAlias;
    if(sdiff < 0){
        qDebug() << "Неверные параметры для построения пирамид";
        return;
    }
    double initSigma = sqrt(sdiff);
    FImage img = prepareImage(initSigma);
    //img.normalize();
    int imageIndex = 0;
    imagesBuffer[imageIndex] = std::move(img);
    double k = pow(2, (1. / levelsPerOctave));
    double effecSigma = sigma;
    for(int i = 0; i < octavesCount; ++i) {
        for(int j = 0; j < levelsPerOctave - 1; ++j) {
            double snext = effecSigma * k;
            double dlt = sqrt(snext * snext - effecSigma * effecSigma);
            Kernel kernelX1 = Kernel::createGaussSeparateKernelX(dlt);
            Kernel kernelY1 = Kernel::createGaussSeparateKernelY(dlt);
            FImage& prev = imagesBuffer[imageIndex];
            Convolution convX = Convolution(kernelX1, type);
            Convolution convY = Convolution(kernelY1, type);
            FImage imgX1 = convX.apply(prev);
            FImage imgY1 = convY.apply(imgX1);
            //imgY1.normalize();
            imagesBuffer[++imageIndex] = std::move(imgY1);
            effecSigma = snext;
        }
        effecSigma /= 2;
        if (i < octavesCount - 1) {
            int prevIndex = imageIndex;
            imagesBuffer[++imageIndex] = imagesBuffer[prevIndex].downscale(1);
        }
    }
}

void PyramidBuilder::createDog() {
    int dogSize = octavesCount * (levelsPerOctave - 1);

}
