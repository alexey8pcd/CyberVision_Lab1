#ifndef PYRAMIDBUILDER_H
#define PYRAMIDBUILDER_H
#include "math.h"
#include "qlist.h"
#include "qimage.h"
#include "convolution.h"
#include "qdebug.h"
#include "datatypes.h"

class PyramidBuilder
{
private:
    int octaveCount;
    int levelPerOctave;
    double sigmaAlpha;
public:
    PyramidBuilder(int octaveCount, int levelPerOctave, double sigmaAlpha);
    PyramidBuilder();
    void createOctaves(FImage &image, EdgeType type);
};

#endif // OCTAVE_H
