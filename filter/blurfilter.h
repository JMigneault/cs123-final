#ifndef BLURFILTER_H
#define BLURFILTER_H

#include "ui/Canvas2D.h"
#include "glm.hpp"
#include "glm/gtc/constants.hpp"

class BlurFilter
{
public:
    BlurFilter();

    static void filter(Canvas2D *canvas, int radius);
    static float gaussian(float x, float sigma);
};

#endif // BLURFILTER_H
