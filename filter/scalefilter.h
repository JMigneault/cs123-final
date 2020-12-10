#ifndef SCALEFACTOR_H
#define SCALEFACTOR_H

#include "ui/Canvas2D.h"
#include "glm.hpp"
#include "glm/gtc/constants.hpp"

class ScaleFilter
{
public:
    ScaleFilter();

    static void filter(Canvas2D *canvas, float scaleX, float scaleY);

    static void scaleUpWidth(Canvas2D *canvas, float a);
    static void scaleUpHeight(Canvas2D *canvas, float a);
    static void scaleDownWidth(Canvas2D *canvas, float a);
    static void scaleDownHeight(Canvas2D *canvas, float a);


};

#endif // SCALEFACTOR_H
