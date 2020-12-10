#include "helpers.h"
#include "glm.hpp"
#include "memory.h"

Helpers::Helpers()
{
}

void Helpers::rgbToRgbFloat(rgbFloat *result, RGBA *data, int length) {
    for (int i = 0; i < length; i++) {
        result[i] = rgbFloat{data[i].r / 255.0f, data[i].g / 255.0f, data[i].b / 255.0f};
    }
}

void Helpers::rgbFloatToRgb(RGBA *result, rgbFloat *data, int length) {
    for (int i = 0; i < length; i++) {
        result[i] = RGBA{realToByte(data[i].r), realToByte(data[i].g), realToByte(data[i].b)};
    }
}

inline int Helpers::index2D(int row, int col, int width) {
    return row * width + col;
}

unsigned char Helpers::rgbaToGray(RGBA pixel) {
    return 0.299f * pixel.r + 0.587f * pixel.g + 0.114f * pixel.b;
}

inline unsigned char Helpers::realToByte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

void Helpers::normalizeKernel(float *kernel, int size) {
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        sum += kernel[i];
    }
    if (sum == 0) {
        return;
    }
    for (int i = 0; i < size; i++) {
        kernel[i] /= sum;
    }

}

int inline adjustIndex(int i, int bound) {

    if (i < 0 || i >= bound) {
        return i >= bound ? 2 * bound - i - 1 : -i - 1;
    }
    return i;
}

void Helpers::conv2D(rgbFloat *result, rgbFloat *data, int width, int height, float *kernel, int kWidth, int kHeight) {

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {

            float red_acc = 0, green_acc = 0, blue_acc = 0;

            for (int i = 0; i < kHeight; i++) {
                for (int j = 0; j < kWidth; j++) {

                    int imageRow = r + i - kHeight / 2;
                    int imageCol = c + j - kWidth / 2;

                    int kernelIndex = index2D(i, j, kWidth);

                    if (imageRow < 0 || imageRow >= height || imageCol < 0 || imageCol >= width) {
                        imageRow = adjustIndex(imageRow, height);
                        imageCol = adjustIndex(imageCol, width);
                    }
                    int imageIndex = index2D(imageRow, imageCol, width);
                    red_acc += kernel[kernelIndex] * data[imageIndex].r;
                    green_acc += kernel[kernelIndex] * data[imageIndex].g;
                    blue_acc += kernel[kernelIndex] * data[imageIndex].b;
                }
            }


            result[index2D(r, c, width)] = rgbFloat{red_acc,
                                                    green_acc,
                                                    blue_acc};

            }
        }

}

