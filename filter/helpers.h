#ifndef HELPERS_H
#define HELPERS_H

#include<vector>
#include "RGBA.h"


class Helpers
{
public:
    Helpers();

    struct rgbFloat {
        float r;
        float g;
        float b;
    };

    static int index2D(int row, int col, int width);
    static unsigned char realToByte(float f);
    static unsigned char rgbaToGray(RGBA pixel);
    static void conv2D(rgbFloat *result, rgbFloat *data, int width, int height, float *kernel, int kWidth, int kHeight);
    static void normalizeKernel(float *kernel, int size);
    static void rgbToRgbFloat(rgbFloat *result, RGBA *data, int length);
    static void rgbFloatToRgb(RGBA *result, rgbFloat *data, int length);

};

#endif // HELPERS_H
