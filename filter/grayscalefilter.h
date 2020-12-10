#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "Canvas2D.h"


class GrayscaleFilter
{
public:
    GrayscaleFilter();

    static void filter(Canvas2D *canvas);
};

#endif // GRAYSCALEFILTER_H
