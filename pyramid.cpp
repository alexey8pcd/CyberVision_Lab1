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

void PyramidBuilder::createOctaves(QImage &image){
//    double k = pow(2,(1. / levelPerOctave));
//    int l = 0;
//    int scale = 1;
//    for(int i = 0; i < octaveCount; ++i) {

//        for(int j = 0; j < levelPerOctave; ++j) {
//            double sigmaNext = sigmaAlpha * pow(k, l++);
//            qInfo() << "Octave: " << (i + 1) << ", Level: "
//                    << (j + 1) << ", Sigma: " << sigmaNext;
//            QImage img = Convolution::applyGauss(image, sigmaNext);
//            if(scale > 1){
//                img = ImageUtil::downscale(img, scale);
//            }
//            img.save(QString::number(i * 10 + j).append(".png"),"png");
//        }
//        scale *= 2;
//    }
}
