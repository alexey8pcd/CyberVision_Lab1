#ifndef PYRAMIDBUILDER_H
#define PYRAMIDBUILDER_H
#include "math.h"
#include "qlist.h"
#include "qimage.h"
#include "convolution.h"
#include "qdebug.h"
#include "datatypes.h"

class PyramidBuilder {
private:
    int levelsPerOctave;
    int octavesCount;
    double sigmaAlias;
    double sigma;
    unique_ptr<FImage[]> imagesBuffer;
    unique_ptr<FImage[]> dogBuffer;
    FImage image;
    EdgeType type;
    PyramidBuilder();
    FImage prepareImage(double initSigma);

    public:
    PyramidBuilder(FImage &image, EdgeType type, int levelsPerOctave,
                   double sigmaAlias, double sigma);
    float getL(int x, int y, float sigma);
    void createOctaves();
    void createOctavesWithAddLevels();
    void createDog();
    void save();
};

#endif
