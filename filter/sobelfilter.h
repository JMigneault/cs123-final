#ifndef SOBELFILTER_H
#define SOBELFILTER_H

#include "ui/Canvas2D.h"
#include "glm.hpp"

class SobelFilter
{
public:
    SobelFilter();

    static void filter(Canvas2D *canvas, float sensitivity);

};

#endif // SOBELFILTER_H
