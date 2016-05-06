#ifndef OCTAVE_H
#define OCTAVE_H
#include "math.h"
#include "qlist.h"
#include "qimage.h"
#include "convolution.h"
#include "qdebug.h"

class PyramidBuilder
{
private:
    int octaveCount;
    int levelPerOctave;
    double sigmaAlpha;
public:
    PyramidBuilder(int octaveCount, int levelPerOctave, double sigmaAlpha);
    PyramidBuilder();
    void createOctaves(QImage &image);
};

#endif // OCTAVE_H
