#include "sobelfilter.h"
#include "helpers.h"
#include "grayscalefilter.h"

SobelFilter::SobelFilter()
{
}

void SobelFilter::filter(Canvas2D *canvas, float sensitivity) {
    int height = canvas->height();
    int width = canvas->width();

    GrayscaleFilter::filter(canvas);

    float kernel1[3] = {-1, 0, 1};
    float kernel2[3] = {1, 2, 1};

    // setup buffers to hold partially filtered results
    std::vector<Helpers::rgbFloat> buffer1(height * width);
    std::vector<Helpers::rgbFloat> buffer2(height * width);
    std::vector<Helpers::rgbFloat> buffer3(height * width);

    Helpers::rgbToRgbFloat(buffer2.data(), canvas->data(), width * height);

    // vertical edges6
    Helpers::conv2D(buffer3.data(), buffer2.data(), width, height, kernel1, 1, 3);
    Helpers::conv2D(buffer1.data(), buffer3.data(), width, height, kernel2, 3, 1);

    // horizontal edges
    Helpers::conv2D(buffer3.data(), buffer2.data(), width, height, kernel2, 1, 3);
    Helpers::conv2D(buffer2.data(), buffer3.data(), width, height, kernel1, 3, 1);

    // add intensities across the vert and horz images in quadrature and gate at (1 - sensitivity)
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            int i = Helpers::index2D(r, c, width);
            buffer3[i].r = sqrt(buffer1[i].r * buffer1[i].r + buffer2[i].r * buffer2[i].r) * sensitivity;
            buffer3[i].g = sqrt(buffer1[i].g * buffer1[i].g + buffer2[i].g * buffer2[i].g) * sensitivity;
            buffer3[i].b = sqrt(buffer1[i].b * buffer1[i].b + buffer2[i].b * buffer2[i].b) * sensitivity;
        }
    }

    Helpers::rgbFloatToRgb(canvas->data(), buffer3.data(), width * height);

}

