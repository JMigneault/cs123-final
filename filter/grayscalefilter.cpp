#include "grayscalefilter.h"
#include "helpers.h"

GrayscaleFilter::GrayscaleFilter()
{

}

void GrayscaleFilter::filter(Canvas2D *canvas) {
    int height = canvas->height();
    int width = canvas->width();

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            int i = Helpers::index2D(r, c, width);
            RGBA test = canvas->data()[i];
            unsigned char intensity = Helpers::rgbaToGray(test);
            canvas->data()[i] = RGBA(intensity, intensity, intensity);
        }
    }

}
